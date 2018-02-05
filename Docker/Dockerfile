FROM ubuntu
#the basic runtime enviornment is created from an ubuntu image
#which avaitable in public repository
ADD lib.tar.xz /
#to run kgwrite we need extra libraries which not available in ubuntu
#which can be found out by running 'ldd kgwrite'
#only libraries not available in ubuntu are included
#docker will untar an .xz tab ball
#VOLUME ["/home"]
#telling that /home is a mount point, used to mount /home
#of host to mount on 'home' in the new environment
#used to make host files available at runtime to read and write
RUN groupadd -g 1001 babu;useradd -g 1001 -u 1000 -b /home -m babu;
#adding a user and group so newly created files will be
#is with user:gruop
ADD kgwrite /
#adding kgwrite
#ADD authorized_keys /
ADD scp /usr/bin/
ADD ssh /usr/bin/
ADD git /usr/bin/
ADD authorized_keys /
RUN ldconfig ; 
#running ldconfig in 'root' mode itself to update the included libraries
WORKDIR /home/babu
#declaring the work directory as /home/babu so files will be read add
#written relative to this
USER babu:babu
#changing to user and group so that file ownership is okay
RUN rm -rf data;mkdir .ssh;cp /authorized_keys .ssh/;
RUN git clone ssh:babu@127.0.0.1:BUILD/kgwrite data
CMD ["/kgwrite","-fReadme.ps","data/Readme"]
#actual command to run
