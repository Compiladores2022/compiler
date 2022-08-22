docker build --platform linux/x86_64 -t ubuntu20:1 .
docker run -it -d -v ~/Documents/university/compiladores/preproyecto--nolasco-parladorio:/home/usr/compilers -e TERM=xterm-256color --name ubuntu20 ubuntu20:1
