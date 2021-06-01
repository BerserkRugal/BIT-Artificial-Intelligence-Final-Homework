import numpy as np
import tensorflow as tf
import os

os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

class NetWork2(tf.keras.Model):
    def __init__(self, board_width, model_file=None):
        super(GobangPolicyValueNet2, self).__init__()
        
        self.board_width = board_width
        self.learning_rate_fn=0.001
        self.input_states = tf.keras.layers.Input([4, board_width, board_width], dtype='float32', name='input_states')  # TODO C plain x 2
        self.input_state = tf.transpose(self.input_states, [0, 2, 3, 1], name="input_state")
        X = tf.keras.layers.Conv2D(filters=256, kernel_size=[3, 3], padding="same", data_format="channels_last")(self.input_state)
        X = tf.keras.layers.BatchNormalization(epsilon=1e-5, fused=True)(X)
        X = tf.nn.relu(X)
        def residual_block(input):
            X = tf.keras.layers.Conv2D(filters=256, kernel_size=[3, 3], padding="same", data_format="channels_last")(input)
            X = tf.keras.layers.BatchNormalization(epsilon=1e-5, fused=True)(X)
            X = tf.nn.relu(X)

            X = tf.keras.layers.Conv2D(filters=256, kernel_size=[3, 3], padding="same", data_format="channels_last")(X)
            X = tf.keras.layers.BatchNormalization(epsilon=1e-5, fused=True)(X)

            add = tf.add(X, input)
            return tf.nn.relu(add)

        for i in range(7):
            X = residual_block(X)
        Y = tf.keras.layers.Conv2D(filters=2, kernel_size=[1, 1], padding="same", data_format="channels_last")(X)
        Y = tf.keras.layers.BatchNormalization(epsilon=1e-5, fused=True)(Y)
        Y = tf.nn.relu(Y)
        self.action_conv_flat = tf.reshape(Y, [-1, 2 * board_width * board_width], name="action_conv_flat")
        self.action_fc = tf.keras.layers.Dense(board_width * board_width,activation=tf.nn.log_softmax)(self.action_conv_flat)
        Y = tf.keras.layers.Conv2D(filters=1, kernel_size=[1, 1], padding="same", data_format="channels_last")(X)
        Y = tf.keras.layers.BatchNormalization(epsilon=1e-5, fused=True)(Y)
        Y = tf.nn.relu(Y)
        self.evaluation_conv_flat = tf.reshape(Y, [-1, 1 * board_width * board_width], name="evaluation_conv_flat")
        self.evaluation_fc1 = tf.keras.layers.Dense(64, activation='relu', name="evaluation_fc1")(self.evaluation_conv_flat)
        self.evaluation_fc2 = tf.keras.layers.Dense(1, activation='tanh', name="evaluation_fc2")(self.evaluation_fc1)

        self.model = tf.keras.Model(
            inputs=[self.input_states],
            outputs=[self.action_fc, self.evaluation_fc2])

        self.model.summary()

        self.global_step = tf.Variable(0, name="global_step", trainable=False)
        self.loss = ""
        self.c_l2 = 0.0001
        self.optimizer = tf.compat.v1.train.AdamOptimizer(learning_rate=self.learning_rate_fn, name="optimizer")
        self.checkpoint_prefix = os.path.join(model_file, 'ckpt')
        self.checkpoint = tf.train.Checkpoint(model=self.model, optimizer=self.optimizer)
        self.checkpoint.restore(tf.train.latest_checkpoint(model_file))

    def predict(self, state_batch):
        state_batch=np.array(state_batch)
        state_batch = state_batch.astype(np.float32)
        if len(state_batch.shape) == 3:
            sp = state_batch.shape
            state_batch=np.reshape(state_batch, [1, sp[0], sp[1], sp[2]])
        log_act_probs, value = self.model(state_batch, training=False)
        act_probs = np.exp(log_act_probs)    #OverflowError: Python int too large to convert to C long  
        return act_probs, value

    def compute_metrics(self, pi_, policy_head):
        # Accuracy
        correct_prediction = tf.equal(tf.argmax(input=policy_head, axis=1), tf.argmax(input=pi_, axis=1))
        correct_prediction = tf.cast(correct_prediction, tf.float32)
        accuracy = tf.reduce_mean(input_tensor=correct_prediction, name='accuracy')
        return accuracy
    
    def apply_regularization(self, regularizer, weights_list=None):
        if not weights_list:
            raise ValueError('No weights to regularize.')
        with tf.name_scope('get_regularization_penalty') as scope:
            penalties = [regularizer(w) for w in weights_list]
            penalties = [
                p if p is not None else tf.constant(0.0) for p in penalties
            ]
            for p in penalties:
                if p.get_shape().ndims != 0:
                    raise ValueError('regularizer must return a scalar Tensor instead of a '
                                     'Tensor with rank %d.' % p.get_shape().ndims)

            summed_penalty = tf.add_n(penalties, name=scope)
            return summed_penalty


    def compute_loss(self, pi_, z_, policy_head, value_head):
        # loss
        with tf.name_scope("loss"):
            policy_loss = tf.keras.losses.categorical_crossentropy(y_true=pi_, y_pred=policy_head, from_logits=False)
            policy_loss = tf.reduce_mean(policy_loss)

            value_loss = tf.keras.losses.mean_squared_error(z_, value_head)
            value_loss = tf.reduce_mean(value_loss)

            regularizer = tf.keras.regularizers.l2(self.c_l2)
            regular_variables = self.model.trainable_variables
            l2_loss = self.apply_regularization(regularizer, regular_variables)

            self.loss = value_loss + policy_loss + l2_loss

        return self.loss


    @tf.function
    def train(self, positions, pi, z, learning_rate=0.001):

        with tf.GradientTape() as tape:
            log_act_probs, value_head = self.model(positions, training=True)
            policy_head = tf.exp(log_act_probs)
            loss = self.compute_loss(pi, z, policy_head, value_head)
            metrics = self.compute_metrics(pi, policy_head)
        grads = tape.gradient(loss, self.model.trainable_variables)

        self.optimizer.apply_gradients(zip(grads, self.model.trainable_variables),global_step=self.global_step, name='train_step')

        return loss, metrics

    def save_model(self, in_global_step):
        self.checkpoint.save(self.checkpoint_prefix)



