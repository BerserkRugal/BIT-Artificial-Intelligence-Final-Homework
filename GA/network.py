import random
import logging
import train


#nn_param_choices = {
#       'nb_kernals':5,
#       'nb_Conv2D_32_layers':2,
#       'nb_Conv2D_64_layers':1,
#       'nb_Conv2D_128_layers':1,
#       'nb_Dense_layers':2,
#       'nb_neurons':512,
#       'activation': 'relu',
#       'optimizer': 'adam',
#    }
#train.train_and_score(nn_param_choices)

class Network():
 
     #Initialize our network.
     #    Args:
     #       nn_param_choices (dict): Parameters for the network, includes:
     #           'nb_kernals':[3,5],
     #           'nb_Conv2D_32_layers':[1,2],
     #           'nb_Conv2D_64_layers':[1,2],
     #           'nb_Conv2D_128_layers':[0,1,2],
     #           'nb_Dense_layers':[1,2,3],
     #           'nb_neurons':[256, 512, 768, 1024],
     #           'activation': ['relu', 'elu', 'tanh', 'sigmoid'],
     #           'optimizer':['rmsprop', 'adam', 'sgd', 'adagrad','adadelta', 'adamax', 'nadam']
    def __init__(self, nn_param_choices=None):         
        self.accuracy = 0.
        self.nn_param_choices = nn_param_choices
        self.network = {} 

    # Create a random network.
    def create_random(self):
        for key in self.nn_param_choices:
            self.network[key] = random.choice(self.nn_param_choices[key])

    # Set network properties.
    def create_set(self, network):
        self.network = network

    # Train the network and record the accuracy.
    def train(self):
        if self.accuracy == 0.:
            self.accuracy = train.train_and_score(self.network)

    # Print out a network.
    def print_network(self):
        logging.info(self.network)
        logging.info("Network accuracy: %.2f%%" % (self.accuracy * 100))