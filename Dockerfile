# Base image
FROM ubuntu AS buildstage

# Install build tools
RUN apt-get update --fix-missing && apt-get upgrade -y
RUN apt-get -y install \
    cmake pkg-config doxygen \
    libopenmpi-dev

# Change working directory
WORKDIR /src

# Copy the current folder which contains C++ source code to the Docker image
COPY . .

# Use cmake to compile the cpp source file. cmake@v3.10.x .
RUN cmake -E make_directory build && \
    cmake -E chdir ./build cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build ./build

# # Check if the program has dynamic dependencies
# RUN cd /src/bin && \ 
#     ldd tutorial && \
#     ldd mandel && \
#     ldd linked && \
#     ldd prodCons && \
#     ldd pi_mc

# # Multistage build
# FROM alpine AS runtime

# # Label
# LABEL author Adaickalavan

# # Copy needed source
# COPY --from=buildstage /src/build /src/build/
# COPY --from=buildstage /src/bin /src/bin/

# # Command to execute when the image loads
# ENTRYPOINT ["/src/bin/tutorial"]