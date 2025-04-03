FROM d3lta12/ubuntu-build-tools:latest

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt update
RUN apt install --no-install-recommends -y \
    libsdl2-dev \
    python3-catkin-pkg \
    python3-empy \
    python3-nose \
    python3-setuptools \
    libgtest-dev

# Install Catkin
WORKDIR /opt
RUN git clone https://github.com/ros/catkin.git
RUN mkdir /opt/catkin/build
WORKDIR /opt/catkin/build
RUN cmake .. && make && make install
RUN echo "source /opt/catkin/build/devel/setup.bash" >> ~/.bashrc
RUN echo ". /opt/catkin/build/devel/setup.sh" >> ~/.profile

# Install Serial
WORKDIR /opt
RUN git clone https://github.com/wjwwood/serial.git
WORKDIR /opt/serial
RUN . /opt/catkin/build/devel/setup.sh && make
RUN cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local && make install

# Cleanup
WORKDIR /root
RUN apt clean
RUN rm -rf /tmp/* /var/tmp/* /var/lib/apt/lists/*