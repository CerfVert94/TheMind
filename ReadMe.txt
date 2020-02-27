[IMPORTANT]install npm and cmake beforehand.

Enter following lines in a terminal.
./install.sh
./launch.sh

Then, enter following URLs in a web browser :
http://localhost:8000/index.html?name=Alice
http://localhost:8000/index.html?name=Bob
http://localhost:8000/index.html?name=Carl
http://localhost:8000/index.html?name=David

4 clients are necessary to start the game.

After launching CMake, the dynamic library (libmosquitto.so.1.6.8) should be generated in ./mqtt_lib/lib

To launch manually the game host of TheMind, 
enter following commands in the Host directory:
LD_LIBRARY_PATH=../mqtt_lib/lib ./exe/Host
