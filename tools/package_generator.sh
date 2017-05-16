#! /usr/local/bin/bash

# ========================================================
# SETUP
# ========================================================
pushd `dirname ${BASH_SOURCE[0]}`/..

if [[ $1 == '-h' || $1 == '--help' ]] ; then
    echo "Usage: package_generator [version]"
    echo "Will push to S3 with an appropriate name based on [version]. If no argument is used, will not push to S3"
    exit 0
fi

version=$1

repo_dir=.
out_dir=/tmp/jumper-ulogger-package-generator


# ========================================================
# NRF52 PACKAGE
# ========================================================
nrf52_package_dir_name=jumper_ulogger
nrf52_package_dir=${out_dir}/${nrf52_package_dir_name}

if [ -z $version ] ; then
    nrf52_package_zip=${out_dir}/jumper_ulogger_nrf52.zip
else
    nrf52_package_zip=${out_dir}/jumper_ulogger_nrf52-${version}.zip
fi

mkdir -p ${nrf52_package_dir}

cp ${repo_dir}/ulogger/*.c ${nrf52_package_dir}
cp ${repo_dir}/ulogger/*.h ${nrf52_package_dir}

mkdir ${nrf52_package_dir}/handlers
cp ${repo_dir}/ulogger/handlers/*.h ${nrf52_package_dir}/handlers
cp ${repo_dir}/ulogger/handlers/*.c ${nrf52_package_dir}/handlers

cp ${repo_dir}/platforms/nrf52/*.c ${nrf52_package_dir}
cp ${repo_dir}/platforms/nrf52/*.h ${nrf52_package_dir}

pushd ${nrf52_package_dir}/..
zip -r ${nrf52_package_zip} `basename ${nrf52_package_dir}`
popd

if [ ! -z $version ] ; then
    s3cmd put ${nrf52_package_zip} s3://jumper-public/ulogger/
fi


# ========================================================
# NRF52 SAMPLE PROJECT
# ========================================================
nrf52_sample_dir_src=${repo_dir}/samples/nrf52-ble-sample-project
nrf52_sample_dir_dest=${out_dir}/nrf52-ble-sasmple-project
if [ -z $version ] ; then
    nrf52_sample_zip=${out_dir}/nrf52-ble-sasmple-project.zip
else
    nrf52_sample_zip=${out_dir}/nrf52-ble-sasmple-project-${version}.zip
fi
nrf52_sample_ulogger_dir=${nrf52_sample_dir_dest}/${nrf52_package_dir_name}

pushd ${nrf52_sample_dir_src}
make clean
popd

cp -r ${nrf52_sample_dir_src} ${nrf52_sample_dir_dest}
mkdir -p ${nrf52_sample_ulogger_dir}

cp -r $nrf52_package_dir $nrf52_sample_dir_dest/

sed -E -i "" "s/^.*ULOGGER_DIR +:=.*\$/ULOGGER_DIR := ${nrf52_package_dir_name}/g" ${nrf52_sample_dir_dest}/Makefile
sed -E -i "" "s/^.*PLATFORM_DIR +:=.*\$/PLATFORM_DIR := ${nrf52_package_dir_name}/g" ${nrf52_sample_dir_dest}/Makefile

pushd ${nrf52_sample_dir_dest}/..
zip -r ${nrf52_sample_zip} `basename ${nrf52_sample_dir_dest}`
popd

if [ ! -z $version ] ; then
    s3cmd put ${nrf52_sample_zip} s3://jumper-public/ulogger/
fi


# ========================================================
# Tear Down
# ========================================================
popd