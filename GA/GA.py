# Genetic algorithm is used to 
# complete the evolution of neural network

from functools import reduce
from operator import add
import random
from network import Network

class Inheritance():

    # Args:
    #        nn_param_choices (dictionary): Possible network paremters
    #        retain (float): Percentage of population to retain after each generation
    #        random_select (float): Probability of a rejected network remaining in the population
    #        mutate_chance (float): Probability a network will be randomly mutated
    def __init__(self, nn_param_choices, retain=0.4,
                 random_select=0.1, mutate_chance=0.2):
        self.mutate_chance = mutate_chance
        self.random_select = random_select
        self.retain = retain
        self.nn_param_choices = nn_param_choices

    #  Create a population of random networks.
    #  count: the size of the population
    #  Returns: 
    #      (list): Population
    def create_population(self, count):
        population = []
        for _ in range(0, count):
            # Create a random network.
            network = Network(self.nn_param_choices)
            network.create_random()

            # Add the network to our population.
            population.append(network)

        return population

    # Return the fitness of a network, which is the network's accuracy
    @staticmethod
    def fitness(network):
        return network.accuracy

    # Find average fitness for a population.
    # Return the average accuracy of the population
    def grade(self, population):
        summed = reduce(add, (self.fitness(network) for network in population))
        return summed / float((len(population)))

    # Breed function
    # Args:
    #     mother (dict): Network parameters
    #     father (dict): Network parameters
    # Returns:
    #     (list): Two network objects
    def breed(self, mother, father):
        children = []
        for _ in range(2):

            child = {}

            # Crossover operation (core function)
            for param in self.nn_param_choices:
                child[param] = random.choice(
                    [mother.network[param], father.network[param]]
                )

            # Create a network 
            network = Network(self.nn_param_choices)
            network.create_set(child)

            # Randomly choose some of the children to mutate.
            if self.mutate_chance > random.random():
                network = self.mutate(network)

            children.append(network)

        return children

    # Randomly mutate one part of the network.
    def mutate(self, network):
        # Choose a random key.
        mutation = random.choice(list(self.nn_param_choices.keys()))

        # Mutate one of the params.
        network.network[mutation] = random.choice(self.nn_param_choices[mutation])

        return network

    # Evolve a population of networks.
    def evolve(self, population):

        # Get scores for each network.
        graded = [(self.fitness(network), network) for network in population]

        # Sort on the scores.
        graded = [x[1] for x in sorted(graded, key=lambda x: x[0], reverse=True)]

        # Get the number we want to keep for the next gen.
        retain_length = int(len(graded)*self.retain)

        # The network we want to keep become the parents.
        parents = graded[:retain_length]

        # For those we aren't keeping, randomly keep some anyway.
        for individual in graded[retain_length:]:
            if self.random_select > random.random():
                parents.append(individual)

        # Now find out how many spots we have left to fill.
        parents_length = len(parents)
        desired_length = len(population) - parents_length
        children = []

        # Add children, which are bred from two remaining networks.
        while len(children) < desired_length:

            # Get a random mom and dad.
            male = random.randint(0, parents_length-1)
            female = random.randint(0, parents_length-1)

            # Assuming they aren't the same network...
            if male != female:
                male = parents[male]
                female = parents[female]

                # Breed them.
                babies = self.breed(male, female)

                # Add the children one at a time.
                for baby in babies:
                    # Don't grow larger than desired length.
                    if len(children) < desired_length:
                        children.append(baby)

        parents.extend(children)

        return parents

