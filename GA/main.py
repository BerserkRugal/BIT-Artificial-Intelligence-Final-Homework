# Now lets start evolving ......

import logging
from GA import Inheritance
from tqdm import tqdm

# Setup logging.
logging.basicConfig(
    format='%(asctime)s - %(levelname)s - %(message)s',
    datefmt='%m/%d/%Y %I:%M:%S %p',
    level=logging.DEBUG,
    filename='log.txt'
)
# Train each network, and use the progress bar to display the progress
def train_networks(networks):
    pbar = tqdm(total=len(networks))
    for network in networks:
        network.train()
        pbar.update(1)
    pbar.close()

# Get the average accuracy for a group of networks.
def get_average_accuracy(networks):
    total_accuracy = 0
    for network in networks:
        total_accuracy += network.accuracy

    return total_accuracy / len(networks)

# Generate a network with the genetic algorithm.
# Args:
#    generations (int): Number of times to evole the population
#    population (int): Number of networks in each generation
#    nn_param_choices (dict): Parameter choices for networks
def generate(generations, population, nn_param_choices):
    inheritance = Inheritance(nn_param_choices)
    networks = inheritance.create_population(population)

    # Evolve the generation.
    for i in range(generations):
        logging.info("***Doing generation %d of %d***" %
                     (i + 1, generations))

        # Train and get accuracy for networks.
        train_networks(networks)

        # Get the average accuracy for this generation.
        average_accuracy = get_average_accuracy(networks)

        # Print out the average accuracy each generation.
        logging.info("Generation average: %.2f%%" % (average_accuracy * 100))
        logging.info('-'*80)

        # Evolve, except on the last iteration.
        if i != generations - 1:
            # Do the evolution.
            networks = inheritance.evolve(networks)

    # Sort our final population.
    networks = sorted(networks, key=lambda x: x.accuracy, reverse=True)

    # Print out the top 5 networks.
    print_networks(networks[:5])

# Print a list of networks.
def print_networks(networks):
    logging.info('-'*80)
    for network in networks:
        network.print_network()

def main():

    generations = 10  # Number of times to evole the population.
    population = 20  # Number of networks in each generation.

    nn_param_choices = { 
                'nb_kernals':[3,5],
                'nb_Conv2D_32_layers':[1,2],
                'nb_Conv2D_64_layers':[1,2],
                'nb_Conv2D_128_layers':[0,1,2],
                'nb_Dense_layers':[1,2,3],
                'nb_neurons':[256, 512, 768, 1024],
                'activation': ['relu', 'elu', 'tanh', 'sigmoid'],
                'optimizer':['rmsprop', 'adam', 'sgd', 'adagrad','adadelta', 'adamax', 'nadam'],
               }
    logging.info("***Evolving %d generations with population %d***" %
                 (generations, population))

    generate(generations, population, nn_param_choices)

if __name__ == '__main__':
    main()

