# WeatherStation
This project is designed to predict precipitation by collecting environmental data and using the PyTorch machine learning framework (specifically with neural networks) to recognize specific environmental flags that signify rain. The concept is simple, set up an external node to sit and collect data while semi-frequently transmitting that data to a central hub. The central hub then takes that data and runs it through the neural network algorithm, checking back at data corresponding to preset time intervals. This way we can use the environmental data to predict rain 15 mins, 1 hour, 6 hours, or 1 day ahead. Since the idea is to record data every 15 minutes, we know where to find the necessary data as well as the labels. Another idea is to separate the incoming data into different arrays so that each piece of data gets moved through the arrays (of varying length depending on the predictor of 15 mins, 1 hour, etc) and becomes relevant as the new data comes in. Either way, there should be one set of environmental data for each time interval and only a single output, with a value between 0 and 1 that scores the likelihood of precipitation for the time intervals.

Currently, I have a prototype version of the machine learning working with data from another project (my Skidless repository) but I don't have the hardware necessary to record all the environmental variables. The most challenging sensor to build will be the wind speed sensor; while not difficult in prinicple, getting all the necessary components and creating the process to monitor the sensor is a bit of a hassle. That said, I also still need a barometric pressure sensor and a temperature sensor, I just have decided to work on other, more important things right now. Evntually I would like to get back to this project, but for the time being, I have tabled it.
