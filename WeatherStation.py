import csv
import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import transforms, datasets

# Default variable type for our torch tensors
dtype = torch.float

# Running on the gpu; to change to cpu, switch "cuda:0" to "cpu"
device = torch.device("cuda:0")

# Creating a debug variable to prevent unnecssary printing
DEBUG = False
torch.autograd.set_detect_anomaly(True)

# Opening and reading the historical dataset file to create an initial model
weatherTrainFile = open("C:/Users/grant/Desktop/SoftwareProjects/WeatherStation/InitialWeatherData_Train.csv", "r")
histTrainData = weatherTrainFile.read()
weatherTrainFile.close()

# Remove the csv formatting from the file
histTrainData = histTrainData.split(',')

# Creating params for our model defining the size of each layer
H1, H2, Num_In_Nodes, Num_Out_Nodes = 50, 20, 5, 2

# Creating the model for predicting weather
model = nn.Sequential(
    nn.Linear(Num_In_Nodes, H1).cuda(),
    nn.ReLU().cuda(),
    nn.Linear(H1, H2).cuda(),
    nn.ReLU().cuda(),
    nn.Linear(H2, Num_Out_Nodes).cuda(),
    nn.LogSoftmax(dim=1).cuda()
)

# Setting params for optimizer
Learning_Rate = 5e-2

# Creating an instance of the optimizer
optimizer = optim.SGD(model.parameters(), lr=Learning_Rate, momentum=0.9)
optimizer.zero_grad()

# Setting final params for training
epoch = 2

###########################################################################
# Training the neural network to the sequential model based on data
# provided in the csv file
print("-------------------------------------")
print("Starting training phase...")

for year in range(epoch):
    print()
    print("--")
    print("Iteration ", year+1)
    # Reset the counter for the total number of iterations
    iter_counter = 0

    # Now we load the historical weather data into the trainloader
    trainloader =  torch.utils.data.DataLoader(histTrainData, batch_size=6, shuffle=False)
    # We use the data in the trainloader to iteratively cycle through and update the NN
    for data_point in trainloader:

        # Resetting the optimizer's gradient for a new image batch
        optimizer.zero_grad()

        tensorData = torch.FloatTensor(data_point[1:], dtype=dtype, device=device)
        print(tensorData)

        # Now run the datapoint through the model
        nn_guess = model(tensorData).cuda()

        # Calculating negative log-likelihood loss of the most recent batch
        loss = nn.functional.nll_loss(nn_guess, data_point[0].cuda()).cuda()

        # Backprop to calculate gradients for each of the weights
        loss.backward()

        # Then use the optimizer to update the weight arrays based on the calculated gradients
        optimizer.step()

        # Prints out the loss every 100 iterations
        if iter_counter % 100 == 0:
            print()
            print("Minibatch: ", iter_counter)
            print("Loss: ", loss.item())
        iter_counter += 1
    #loop
#loop
