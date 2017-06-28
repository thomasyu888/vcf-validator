FROM ubuntu:xenial

RUN apt-get update -y && apt-get upgrade -y && apt-get install -y \
build-essential \
cmake \
libboost-dev \
libboost-filesystem-dev \
libboost-program-options-dev \
libboost-regex-dev \
libsqlite3-dev \
ragel \
wget

# Ubuntu-native ODB packages don't seem to work due to a broken ABI, see this thread: http://codesynthesis.com/pipermail/odb-users/2016-May/003277.html
RUN wget http://www.codesynthesis.com/download/odb/2.4/odb_2.4.0-1_amd64.deb -O /opt/odb_2.4.0-1_amd64.deb
RUN wget http://codesynthesis.com/download/odb/2.4/libodb-2.4.0.tar.bz2 -O /opt/libodb.tar.bz2
RUN wget http://codesynthesis.com/download/odb/2.4/libodb-sqlite-2.4.0.tar.bz2 -O /opt/libodb-sqlite.tar.bz2

# ODB compiler
RUN cd /opt && tar jxvf libodb.tar.bz2 && tar jxvf libodb-sqlite.tar.bz2 && dpkg -i odb_2.4.0-1_amd64.deb
RUN cd /opt/libodb-2.4.0 && ./configure && make && make install
RUN cd /opt/libodb-sqlite-2.4.0 && ./configure --with-libodb=../libodb-2.4.0 && make && make install

#COPY ./entrypoint.sh /


RUN cd / && wget https://github.com/EBIvariation/vcf-validator/archive/v0.5.tar.gz
RUN tar -zxvf /v0.5.tar.gz
RUN for i in 1 2 3 ; do ragel -G2 -o /vcf-validator-0.5/inc/vcf/validator_detail_v4${i}.hpp /vcf-validator-0.5/src/vcf/vcf_v4${i}.ragel ; done

RUN mkdir /vcf-validator-0.5/build
RUN cd /vcf-validator-0.5/build && cmake -DBUILD_STATIC=1 .. && make

CMD ["/vcf-validator-0.5/build/bin/vcf_validator"]
