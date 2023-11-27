FROM nlknguyen/alpine-mpich:onbuild

# # ------------------------------------------------------------
# # Build MPI project
# # ------------------------------------------------------------

# Put all build steps and additional package installation here

# Note: the current directory is ${WORKDIR:=/project}, which is
# also the default directory where ${USER:=mpi} will SSH login to

# Copy the content of `project` directory in the host machine to 
# the current working directory in this Docker image
COPY project/ .

# Normal build command
# RUN mpicc -o mpi_hello_world mpi_hello_world.c
#RUN mpicc -o blocking blocking.c
#RUN mpic++ -o Q1a Q1a.cpp
#RUN mpic++ -o Q1b Q1b.cpp
#RUN mpic++ -o Q1c Q1c.cpp
#RUN mpic++ -o Q1d Q1d.cpp
#RUN mpic++ -o Q2 Q2.cpp




# ####################
# For Docker beginner:

# After Docker syntax `RUN`, you can execute any command available in 
# the current shell of the image.

# To switch to root:    USER root
# To switch back to default user: USER ${USER}
