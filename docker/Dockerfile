FROM debian:stretch-slim

RUN apt-get update -y && apt-get install -y --no-install-recommends \
g++ \
make \
cmake \
libbz2-dev \
libboost-dev \
libboost-filesystem-dev \
libboost-iostreams-dev \
libboost-program-options-dev \
libboost-regex-dev \
libboost-log-dev \
libsqlite3-dev \
zlib1g-dev \
ragel \
# Clean up to reduce layer size
&& apt-get clean \
&& rm -rf /var/lib/apt/lists/* /usr/share/doc /usr/share/doc-base

# apt-get update needed again, as it was removed before
RUN apt-get update && apt-get install -y --no-install-recommends wget bzip2 libcurl4-openssl-dev curl build-essential \
# Built-in ODB packages don't seem to work due to a broken ABI, see this thread: http://codesynthesis.com/pipermail/odb-users/2016-May/003277.html
&& wget --no-check-certificate https://www.codesynthesis.com/download/odb/2.4/odb_2.4.0-1_amd64.deb -O /opt/odb_2.4.0-1_amd64.deb \
&& wget --no-check-certificate https://codesynthesis.com/download/odb/2.4/libodb-2.4.0.tar.bz2 -O /opt/libodb.tar.bz2 \
&& wget --no-check-certificate https://codesynthesis.com/download/odb/2.4/libodb-sqlite-2.4.0.tar.bz2 -O /opt/libodb-sqlite.tar.bz2 \
# Build ODB dependencies
&& cd /opt && tar jxvf libodb.tar.bz2 && tar jxvf libodb-sqlite.tar.bz2 && dpkg -i odb_2.4.0-1_amd64.deb \
&& cd libodb-2.4.0 && ./configure && make && make install && cd .. \
&& cd libodb-sqlite-2.4.0 && ./configure --with-libodb=../libodb-2.4.0 && make && make install && cd .. \
# Clean up to reduce layer size
&& apt-get clean && apt-get remove --purge -y bzip2 \
&& rm -rf /opt /var/lib/apt/lists/* /usr/share/doc /usr/share/doc-base
#COPY ./entrypoint.sh /

WORKDIR /
RUN wget --no-check-certificate https://github.com/EBIvariation/vcf-validator/archive/v0.9.2.tar.gz
RUN tar -zxvf /v0.9.2.tar.gz
RUN for i in 1 2 3 ; do ragel -G2 -o /vcf-validator-0.9.2/inc/vcf/validator_detail_v4${i}.hpp /vcf-validator-0.9.2/src/vcf/vcf_v4${i}.ragel ; done

RUN mkdir /vcf-validator-0.9.2/build
RUN cd /vcf-validator-0.9.2/build && cmake .. && make

CMD ["/vcf-validator-0.9.2/build/bin/vcf_validator","-i","/TESLA_VCF.vcf"]
