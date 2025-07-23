#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cassert>


static double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

static double sigmoid_derivative(double y) {

    return y * (1.0 - y);
}

static double random_weight(double range=1.0) {
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<double> dist(-range, range);
    return dist(rng);
}


struct Layer {
    std::vector<double> output;              
    std::vector<double> bias;                
    std::vector<std::vector<double>> weight; 
    std::vector<double> delta;              

    Layer(size_t neurons, size_t prev_neurons) {
        output.resize(neurons);
        bias.resize(neurons);
        delta.resize(neurons);
        if (prev_neurons) {
            weight.resize(prev_neurons, std::vector<double>(neurons));
            for (auto &row : weight)
                for (auto &w : row) w = random_weight();
        }
        for (auto &b : bias) b = random_weight();
    }
};

class NeuralNetwork {
    std::vector<Layer> layers; 
    double lr;                 
public:
    explicit NeuralNetwork(const std::vector<size_t> &topology, double learning_rate = 0.5) : lr(learning_rate) {
        assert(topology.size() >= 2);
        layers.reserve(topology.size());
        layers.emplace_back(topology[0], 0); 
        for (size_t i = 1; i < topology.size(); ++i) {
            layers.emplace_back(topology[i], topology[i-1]);
        }
    }


    const std::vector<double>& forward(const std::vector<double> &input) {
        assert(input.size() == layers.front().output.size());
        layers.front().output = input;
        for (size_t l = 1; l < layers.size(); ++l) {
            auto &prev = layers[l-1];
            auto &curr = layers[l];
            for (size_t j = 0; j < curr.output.size(); ++j) {
                double sum = curr.bias[j];
                for (size_t i = 0; i < prev.output.size(); ++i)
                    sum += prev.output[i] * prev.weight[i][j];
                curr.output[j] = sigmoid(sum);
            }
        }
        return layers.back().output;
    }

    void backward(const std::vector<double> &target) {

        auto &outLayer = layers.back();
        assert(target.size() == outLayer.output.size());
        for (size_t k = 0; k < target.size(); ++k) {
            double err = target[k] - outLayer.output[k];
            outLayer.delta[k] = err * sigmoid_derivative(outLayer.output[k]);
        }

        for (size_t l = layers.size() - 2; l > 0; --l) {
            auto &curr = layers[l];
            auto &next = layers[l+1];
            for (size_t i = 0; i < curr.output.size(); ++i) {
                double sum = 0.0;
                for (size_t j = 0; j < next.output.size(); ++j)
                    sum += curr.weight[i][j] * next.delta[j];
                curr.delta[i] = sum * sigmoid_derivative(curr.output[i]);
            }
        }

        for (size_t l = 1; l < layers.size(); ++l) {
            auto &prev = layers[l-1];
            auto &curr = layers[l];
            for (size_t i = 0; i < prev.output.size(); ++i) {
                for (size_t j = 0; j < curr.output.size(); ++j) {
                    double grad = prev.output[i] * curr.delta[j];
                    prev.weight[i][j] += lr * grad;
                }
            }
            for (size_t j = 0; j < curr.output.size(); ++j) {
                curr.bias[j] += lr * curr.delta[j];
            }
        }
    }

    void train(const std::vector<std::vector<double>> &inputs, const std::vector<std::vector<double>> &targets,
               size_t epochs = 10000) {
        assert(inputs.size() == targets.size());
        for (size_t epoch = 0; epoch < epochs; ++epoch) {
            double mse = 0.0;
            for (size_t idx = 0; idx < inputs.size(); ++idx) {
                const auto &out = forward(inputs[idx]);
                backward(targets[idx]);
                for (size_t k = 0; k < out.size(); ++k) {
                    double err = targets[idx][k] - out[k];
                    mse += err * err;
                }
            }
            if (epoch % 1000 == 0) {
                std::cout << "Epoch " << epoch << " | MSE: " << mse / inputs.size() << '\n';
            }
        }
    }
};


int main() {
    std::vector<std::vector<double>> X = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };
    std::vector<std::vector<double>> Y = {
        {0.0}, {1.0}, {1.0}, {0.0}
    };

    NeuralNetwork nn({2, 2, 1}, 0.5);
    nn.train(X, Y, 10000);

    std::cout << "\nTesting after training:\n";
    for (size_t i = 0; i < X.size(); ++i) {
        const auto &out = nn.forward(X[i]);
        std::cout << X[i][0] << " XOR " << X[i][1] << " = " << out[0] << '\n';
    }
    return 0;
}
