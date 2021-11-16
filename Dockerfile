FROM ubuntu:latest AS base

FROM base AS dev_machine


RUN apt-get update && apt-get -y install sudo cmake git

RUN groupadd -g 999 foo && useradd -u 999 -g foo -G sudo -m -s /bin/bash foo && \
    sed -i /etc/sudoers -re 's/^%sudo.*/%sudo ALL=(ALL:ALL) NOPASSWD: ALL/g' && \
    sed -i /etc/sudoers -re 's/^root.*/root ALL=(ALL:ALL) NOPASSWD: ALL/g' && \
    sed -i /etc/sudoers -re 's/^#includedir.*/## **Removed the include directive** ##"/g' && \
    echo "foo ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers && \
    echo "foo user:";  su - foo -c id


# Make sure tzdata won't hang our script

ENV TZ=America/Chicago

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone


# Run the install_deps cmake script

WORKDIR /

COPY Depsfile Depsfile

RUN git clone https://github.com/benthevining/DepsInstaller.git

RUN DEBIAN_FRONTEND=noninteractive cmake -D DEPFILE=$(pwd)/Depsfile -D CAT=Docker -P DepsInstaller/install_deps.cmake

RUN DEBIAN_FRONTEND=noninteractive sudo apt-get update && sudo apt-get upgrade

RUN rm -rf DepsInstaller/*

# Make sure clang is the default compiler

RUN DEBIAN_FRONTEND=noninteractive update-alternatives --install /usr/bin/cc cc /usr/bin/clang-11 100 && \
    DEBIAN_FRONTEND=noninteractive update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-11 100
