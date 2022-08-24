FROM ubuntu:20.04

RUN apt update
RUN apt -y upgrade
RUN apt-get install -y build-essential
RUN apt-get install -y flex
RUN apt-get install -y bison
RUN apt-get install -y vim

RUN mkdir -p /home/usr/compilers
WORKDIR /home/usr/

CMD ["bash"]
