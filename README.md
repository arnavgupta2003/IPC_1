<H1 align = "center"> InterProcess Communication in Linux </H1>
<p>
An independent process is not affected by the execution of other processes while a co-operating process can be affected by other executing processes.  
</p>

## Q2) InterProcess Communication
<p align="Left">
Two programs P1 and P2. The first program P1 needs to generate an
array of 50 random strings (of characters) of fixed length each. P1 then sends a group of five consecutive elements of the array of strings to P2 along with the ID’s of the strings, where the ID is the index of the array corresponding to the string. The second program P2 needs to accept the received strings, and send back the highest ID received back to P1 to acknowledge the strings received. The program P2 simply prints the ID’s and the strings on the console. On receiving the acknowledged packet, P1 sends the next five strings, with the string elements starting from the successor of the acknowledged ID.
</p>

## `Directory Structure`

```console
  src/2
        /FIFO
              /P1.c
              /P2.c
              /Makefile
        /SHM
              /P1.c
              /P2.c  
              /Makefile 
        /SOCKET
              /P1.c
              /P2.c 
              /Makefile        
```
## `Running the Programs`

```console
# run the Makefile
$> make -f Makefile
       OR
$> make

# run the Programs

! In Two Separate Terminals

$> ./p2 (Terminal 1)
       AND
$> ./p1 (Terminal 2)

! In this order


```


#### Q2)(A) Using FIFO
For this Implementation two FIFOs have been created in the following fashion

    P1 writes into FIFO_1
    P2 reads from FIFO_1
    P2 writes into FIFO_2
    P1 reads from FIFO_2

This is done to prevent unnecessary read/writes from FIFOs while the other is trying to communicate.

Firstly ```mkfifo(char *, int)``` is used to create the FIFOs.
Then For every read or write command firstly we open the required FIFO by using ```open(char *, int)``` with necessary specifiers like ```O_WRONLY``` or ```RD_ONLY``` to open the FIFO<br> which returns the file descriptor.
Then to read or write we use ```read(int fd, char *, sizeof(char *))``` and ```write(int fd, char *, sizeof(char *))```<br>

<br>

#### Q2)(B) Using SHM
For this Implementation Shared Memory is utilized in the following fashion

    P1 writes into MEM
    P2 reads from MEM
    P2 writes into MEM
    P1 reads from MEM

This is done to prevent unnecessary read/writes from processes while the other is trying to communicate.

Firstly ```key_t ftok(const char *pathname, int proj_id);``` is used to create the file identied by the given pathname.
Then ```int shmget(key_t key, size_t size, int shmflg);``` is used to link to that specific memory location.
Then we use ```void *shmat(int shmid, const void *shmaddr, int shmflg);``` to access the memory block and use the basic ```read()``` and ```sprintf``` to read and write to the Shared memory.<br>

<br>


#### Q2)(C) Using SOCKETS
For this Implementation one Socket has been created in the following fashion

    P1 writes into ProgSocket
    P2 reads from ProgSocket
    P2 writes into ProgSocket
    P1 reads from ProgSocket

This is done to prevent unnecessary read/writes from programs while the other is trying to communicate.

An ```AF_UNIX``` socket has been used as the interprocess communications are local.<br>
```sockaddr_un structure``` has been used to create and use the Socket which is then initialised by ```AF_UNIX``` by ```addr.sun_family = AF_UNIX``` where addr is the name of the struct. We then create a data socket and connect it to adde using ```connect(data_socket, (const struct sockaddr *) &addr, sizeof(addr))```<br>
The above two steps are done in both P1 and P2<br>
Then we use ```write(data_socket, char *, sizeof(char *))``` and ```read(data_socket, char *, sizeof(char *))``` to read and write using the data socket we made.<br>

<br>



## License

MIT © Arnav Gupta 2022<br/>
Original Creator - [Arnav Gupta](https://github.com/arnavgupta2003)
<br><br>
MIT License

Copyright (c) 2022 Arnav Gupta

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
<br>

<p align=center> --- EOF --- </p>
