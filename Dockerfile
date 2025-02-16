FROM ubuntu:latest

WORKDIR /philosophers

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    python3 \
    python3-pip \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

COPY . /philosophers

RUN make -C /philosophers

RUN chmod +x philo


CMD ["python3", "tests/main.py"]