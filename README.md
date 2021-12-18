# Distributed-Project

#### To build this project the following libraries were used:
* **CppSteg** for stenography, this library was slightly adjusted to better fit our project needs
* **OpenCV** for Image manipulation and as a dependency for CppSteg
* **nlohmann_json** for json encoding containers and to transmit data from the server and client
* **base64** to encode and decode messages to base64 to be transmitted over the udp sockets
* **boost** for container serialization and deserialization
* **SQLite3** for database operations

#### Compilation and Execution:
A CMakeList.txt is provided with the project in order to compile and execute the project

The CMakeList defines 2 executables, the client and the server.

To compile the program you need first to install the following dynamic libraries: boost and opencv

Depending on the operating system used, it would need to be installed differently

For example on Linux you are most likely going to be using apt install

To compile the program perform the following commands:

```
mkdir bin
cd bin
cmake ../
make
```

The program should then create the two binaries needed to run the experiment

### Project Flow:

The experiment is set as such the server is continuously running, and the client binary requests and receives responses from the server

The database is set as portrayed in the following image:

![Alt text](readme_files/schema.png?raw=true "Schema")

The schema itself has been included in schema.txt

All the data is stored in the database, including the base64 encoded image.

The server provides the client with the following APIs and queries the DB accordingly

#### The currently supported APIs are:
* **signin**: signin to the service
* **signup**: signup to the service
* **insert_image**: inserts image along with the accessibility list
* **get_my_images**: returns a list of the image you own on the service
* **get_accessible_images**: returns a list of image ids that you could access
* **getOwnedImage**: fetch an image that you own from the database
* **getAccessibleImage**: fetch an image that you have access to from the database

#### The client.cpp defines the following experiment:
1. the user signs up
2. the user then signs in
3. the user uploads image found in images/input.png
4. the user fetches the list of the ids their own images on the service
5. the user fetches the list of accessible image ids on the service
6. the user fetches their first owned image based on the list provided
7. the user fetches an image they have access to based on the list provided

#### Other Notes:
* The MTU set between server and client is 1023 bytes
* Practically, the original image is stored in the database and when the user requests it, the stenography occurs when the image is requested. If you own the image, the full access list along with the id and any other needed info is embedded using stegonography in JSON format. Otherwise, the number of views you have for the image and the max views is embedded along with the owner username and the id.
* The project could run multiple clients concurrently
* The client and server both log to the stdout, to give you an idea of what's happening under the hood