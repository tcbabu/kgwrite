FROM ubuntu
#the basic runtime enviornment is created from an ubuntu image
#which avaitable in public repository
ADD lib.tar.xz /
#to run kgwrite we need extra libraries which not available in ubuntu
#which can be found out by running 'ldd kgwrite'
#only libraries not available in ubuntu are included
#docker will untar an .xz tab ball
VOLUME ["/home"]
#telling that /home is a mount point, used to mount /home
#of host to mount on 'home' in the new environment
#used to make host files available at runtime to read and write
RUN groupadd -g 1001 babu;useradd -g 1001 -u 1000 babu;
#adding a user and group so newly created files will be
#is with user:gruop
ADD kgwrite /
#adding kgwrite
#ADD authorized_keys /
#ADD scp /usr/bin/
#ADD ssh /usr/bin/
RUN ldconfig ; 
#running ldconfig in 'root' mode itself to update the included libraries
WORKDIR /home/babu
#declaring the work directory as /home/babu so files will be read add
#written relative to this
USER babu:babu
#changing to user and group so that file ownership is okay
CMD ["/kgwrite","-fReadme.ps","Readme"]
#actual command to run
