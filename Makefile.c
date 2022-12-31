client_=client
server_=server
cliSrc=client.c comm.c
serSrc=server.c comm.c
.PHONY:all
all:$(client_) $(server_)

$(client_):$(cliSrc)
    gcc -o $@ $^
$(server_):$(serSrc)
    gcc -o $@ $^

.PHONY:clean
clean:
    rm -rf $(client_) $(server_)