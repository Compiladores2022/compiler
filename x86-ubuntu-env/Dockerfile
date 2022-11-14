FROM ubuntu:20.04

RUN apt update
RUN apt -y upgrade
RUN apt-get install -y build-essential
RUN apt-get install -y flex
RUN apt-get install -y bison
RUN apt-get install -y vim
RUN apt-get install -y python3-pip
RUN pip3 install pytest
RUN pip3 install parametrize_from_file

RUN mkdir -p /home/usr/compilers
WORKDIR /home/usr/

CMD ["bash"]
