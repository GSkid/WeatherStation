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
weatherTrainFile = open("InitialWeatherData_Train.csv", "r")
histTrainData = weatherDataFile.read()
weatherTrainFile.close()

# Creating params for our model defining the size of each layer
H1, H2, Num_in_nodes, Num_out_nodes = 50, 20, 5, 2

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


