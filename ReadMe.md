# The Mind
## [IMPORTANT] Install npm and cmake beforehand.

Enter the following lines in a terminal. <br>
./install.sh <br>
./launch.sh <br>

Then, enter following URLs in a web browser :<br>
http://localhost:8000/index.html?name=Alice <br>
http://localhost:8000/index.html?name=Bob <br>
http://localhost:8000/index.html?name=Carl <br>
http://localhost:8000/index.html?name=David <br>

4 clients are necessary to start the game.

After launching CMake, the dynamic library (libmosquitto.so.1.6.8) should be generated in ./mqtt_lib/lib

To launch the game host of TheMind manually, <br>
enter the following command in the Host directory: <br>
LD_LIBRARY_PATH=../mqtt_lib/lib ./exe/Host
