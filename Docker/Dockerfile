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
RUN groupadd -g 1001 babu;useradd -g 1001 -u 1000 -b /home -m -s /bin/bash -p babu babu;
#RUN groupadd -g 1001 babu;useradd -g 1001 -u 1000  babu;
#adding a user and group so newly created files will be
#is with user:gruop
ADD kgwrite /
#adding kgwrite
#ADD authorized_keys /
ADD route /usr/bin/
ADD scp /usr/bin/
ADD ssh /usr/bin/
ADD git /usr/bin/
ADD hostname /bin/
ADD authorized_keys /
ADD ssh_config /etc/ssh/
ADD runscript /
RUN ldconfig ; 
#RUN hostname hplaptop;
#running ldconfig in 'root' mode itself to update the included libraries
RUN mkdir -p /etc/ssh
#EXPOSE 22
WORKDIR /home/babu
#declaring the work directory as /home/babu so files will be read add
#written relative to this
USER babu:babu
#changing to user and group so that file ownership is okay
RUN mkdir /home/babu/.ssh;cp /authorized_keys /home/babu/.ssh/;
RUN chmod 755 /home/babu/.ssh;chmod 644 /home/babu/.ssh/authorized_keys;
RUN ls -lrta /home/babu;ls -lrt /home/babu/.ssh;rm -rf data;
#RUN git clone babu@192.168.1.115:BUILD/kgwrite data
#CMD ["/kgwrite","-fReadme.ps","data/Readme"]
CMD /runscript
#actual command to run
