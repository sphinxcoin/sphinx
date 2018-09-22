# Copyright (c) 2016 The Bitcoin Core developers
# Copyright (c) 2015-2018 The PIVX developers
# Copyright (c) 2018 The Sphinx developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# What to do
sign=false
verify=false
build=false
setupenv=false

# Systems to build
linux=true
windows=true
osx=true

# Other Basic variables
SIGNER=
VERSSPHX=
commit=false
url=https://github.com/cevap/sphx
proc=2
mem=2000
lxc=true
osslTarUrl=https://github.com/cevap/osslsigncode/archive/v1.7.1.tar.gz
osslPatchUrl=https://github.com/cevap/osslsigncode/releases/download/v1.7.1/osslsigncode-Backports-to-1.7.1.patch
scriptName=$(basename -- "$0")
signProg="gpg --detach-sign"
commitFiles=true

# Help Message
read -d '' usage <<- EOF
Usage: $scriptName [-c|u|v|b|s|B|o|h|j|m|] signer verssphx

Run this script from the directory containing the sphx, gitian-builder, gitian.sigs, and sphx-detached-sigs.

Arguments:
signer          GPG signer to sign each build assert file
verssphx		Verssphx number, commit, or branch to build. If building a commit or branch, the -c option must be specified

Options:
-c|--commit	Indicate that the verssphx argument is for a commit or branch
-u|--url	Specify the URL of the repository. Default is https://github.com/cevap/sphx
-v|--verify 	Verify the gitian build
-b|--build	Do a gitian build
-s|--sign	Make signed binaries for Windows and Mac OSX
-B|--buildsign	Build both signed and unsigned binaries
-o|--os		Specify which Operating Systems the build is for. Default is lwx. l for linux, w for windows, x for osx, a for aarch64
-j		Number of processes to use. Default 2
-m		Memory to allocate in MiB. Default 2000
--kvm           Use KVM instead of LXC
--setup         Setup the gitian building environment. Uses KVM. If you want to use lxc, use the --lxc option. Only works on Debian-based systems (Ubuntu, Debian)
--detach-sign   Create the assert file for detached signing. Will not commit anything.
--no-commit     Do not commit anything to git
-h|--help	Print this help message
EOF

# Get options and arguments
while :; do
    case $1 in
        # Verify
        -v|--verify)
	    verify=true
            ;;
        # Build
        -b|--build)
	    build=true
            ;;
        # Sign binaries
        -s|--sign)
	    sign=true
            ;;
        # Build then Sign
        -B|--buildsign)
	    sign=true
	    build=true
            ;;
        # PGP Signer
        -S|--signer)
	    if [ -n "$2" ]
	    then
		SIGNER=$2
		shift
	    else
		echo 'Error: "--signer" requires a non-empty argument.'
		exit 1
	    fi
           ;;
        # Operating Systems
        -o|--os)
	    if [ -n "$2" ]
	    then
		linux=false
		windows=false
		osx=false
		aarch64=false
		if [[ "$2" = *"l"* ]]
		then
		    linux=true
		fi
		if [[ "$2" = *"w"* ]]
		then
		    windows=true
		fi
		if [[ "$2" = *"x"* ]]
		then
		    osx=true
		fi
		if [[ "$2" = *"a"* ]]
		then
		    aarch64=true
		fi
		shift
	    else
		echo 'Error: "--os" requires an argument containing an l (for linux), w (for windows), x (for Mac OSX), or a (for aarch64)\n'
		exit 1
	    fi
	    ;;
	# Help message
	-h|--help)
	    echo "$usage"
	    exit 0
	    ;;
	# Commit or branch
	-c|--commit)
	    commit=true
	    ;;
	# Number of Processes
	-j)
	    if [ -n "$2" ]
	    then
		proc=$2
		shift
	    else
		echo 'Error: "-j" requires an argument'
		exit 1
	    fi
	    ;;
	# Memory to allocate
	-m)
	    if [ -n "$2" ]
	    then
		mem=$2
		shift
	    else
		echo 'Error: "-m" requires an argument'
		exit 1
	    fi
	    ;;
	# URL
	-u)
	    if [ -n "$2" ]
	    then
		url=$2
		shift
	    else
		echo 'Error: "-u" requires an argument'
		exit 1
	    fi
	    ;;
        # kvm
        --kvm)
            lxc=false
            ;;
        # Detach sign
        --detach-sign)
            signProg="true"
            commitFiles=false
            ;;
        # Commit files
        --no-commit)
            commitFiles=false
            ;;
        # Setup
        --setup)
            setup=true
            ;;
	*)               # Default case: If no more options then break out of the loop.
             break
    esac
    shift
done

# Set up LXC
if [[ $lxc = true ]]
then
    export USE_LXC=1
    export LXC_BRIDGE=lxcbr0
    sudo ifconfig lxcbr0 up 10.0.2.2
fi

# Check for OSX SDK
if [[ ! -e "gitian-builder/inputs/MacOSX10.11.sdk.tar.gz" && $osx == true ]]
then
    echo "Cannot build for OSX, SDK does not exist. Will build for other OSes"
    osx=false
fi

# Get signer
if [[ -n"$1" ]]
then
    SIGNER=$1
    shift
fi

# Get verssphx
if [[ -n "$1" ]]
then
    VERSSPHX=$1
    COMMIT=$VERSSPHX
    shift
fi

# Check that a signer is specified
if [[ $SIGNER == "" ]]
then
    echo "$scriptName: Missing signer."
    echo "Try $scriptName --help for more information"
    exit 1
fi

# Check that a verssphx is specified
if [[ $VERSSPHX == "" ]]
then
    echo "$scriptName: Missing verssphx."
    echo "Try $scriptName --help for more information"
    exit 1
fi

# Add a "v" if no -c
if [[ $commit = false ]]
then
	COMMIT="v${VERSSPHX}"
fi
echo ${COMMIT}

# Setup build environment
if [[ $setup = true ]]
then
    sudo apt-get install ruby apache2 git apt-cacher-ng python-vm-builder qemu-kvm qemu-utils
    git clone https://github.com/cevap/gitian.sigs.git
    git clone https://github.com/cevap/sphx-detached-sigs.git
    git clone https://github.com/devrandom/gitian-builder.git
    pushd ./gitian-builder
    if [[ -n "$USE_LXC" ]]
    then
        sudo apt-get install lxc
        bin/make-base-vm --suite trusty --arch amd64 --lxc
    else
        bin/make-base-vm --suite trusty --arch amd64
    fi
    popd
fi

# Set up build
pushd ./sphx
git fetch
git checkout ${COMMIT}
popd

# Build
if [[ $build = true ]]
then
	# Make output folder
	mkdir -p ./sphx-binaries/${VERSSPHX}

	# Build Dependencies
	echo ""
	echo "Building Dependencies"
	echo ""
	pushd ./gitian-builder
	mkdir -p inputs
	wget -N -P inputs $osslPatchUrl
	wget -N -P inputs $osslTarUrl
	make -C ../sphx/depends download SOURCES_PATH=`pwd`/cache/common

	# Linux
	if [[ $linux = true ]]
	then
            echo ""
	    echo "Compiling ${VERSSPHX} Linux"
	    echo ""
	    ./bin/gbuild -j ${proc} -m ${mem} --commit sphx=${COMMIT} --url sphx=${url} ../sphx/contrib/gitian-descriptors/gitian-linux.yml
	    ./bin/gsign -p $signProg --signer $SIGNER --release ${VERSSPHX}-linux --destination ../gitian.sigs/ ../sphx/contrib/gitian-descriptors/gitian-linux.yml
	    mv build/out/sphx-*.tar.gz build/out/src/sphx-*.tar.gz ../sphx-binaries/${VERSSPHX}
	fi
	# Windows
	if [[ $windows = true ]]
	then
	    echo ""
	    echo "Compiling ${VERSSPHX} Windows"
	    echo ""
	    ./bin/gbuild -j ${proc} -m ${mem} --commit sphx=${COMMIT} --url sphx=${url} ../sphx/contrib/gitian-descriptors/gitian-win.yml
	    ./bin/gsign -p $signProg --signer $SIGNER --release ${VERSSPHX}-win-unsigned --destination ../gitian.sigs/ ../sphx/contrib/gitian-descriptors/gitian-win.yml
	    mv build/out/sphx-*-win-unsigned.tar.gz inputs/sphx-win-unsigned.tar.gz
	    mv build/out/sphx-*.zip build/out/sphx-*.exe ../sphx-binaries/${VERSSPHX}
	fi
	# Mac OSX
	if [[ $osx = true ]]
	then
	    echo ""
	    echo "Compiling ${VERSSPHX} Mac OSX"
	    echo ""
	    ./bin/gbuild -j ${proc} -m ${mem} --commit sphx=${COMMIT} --url sphx=${url} ../sphx/contrib/gitian-descriptors/gitian-osx.yml
	    ./bin/gsign -p $signProg --signer $SIGNER --release ${VERSSPHX}-osx-unsigned --destination ../gitian.sigs/ ../sphx/contrib/gitian-descriptors/gitian-osx.yml
	    mv build/out/sphx-*-osx-unsigned.tar.gz inputs/sphx-osx-unsigned.tar.gz
	    mv build/out/sphx-*.tar.gz build/out/sphx-*.dmg ../sphx-binaries/${VERSSPHX}
	fi
	# AArch64
	if [[ $aarch64 = true ]]
	then
	    echo ""
	    echo "Compiling ${VERSSPHX} AArch64"
	    echo ""
	    ./bin/gbuild -j ${proc} -m ${mem} --commit sphx=${COMMIT} --url sphx=${url} ../sphx/contrib/gitian-descriptors/gitian-aarch64.yml
	    ./bin/gsign -p $signProg --signer $SIGNER --release ${VERSSPHX}-aarch64 --destination ../gitian.sigs/ ../sphx/contrib/gitian-descriptors/gitian-aarch64.yml
	    mv build/out/sphx-*.tar.gz build/out/src/sphx-*.tar.gz ../sphx-binaries/${VERSSPHX}
	popd

        if [[ $commitFiles = true ]]
        then
	    # Commit to gitian.sigs repo
            echo ""
            echo "Committing ${VERSSPHX} Unsigned Sigs"
            echo ""
            pushd gitian.sigs
            git add ${VERSSPHX}-linux/${SIGNER}
            git add ${VERSSPHX}-aarch64/${SIGNER}
            git add ${VERSSPHX}-win-unsigned/${SIGNER}
            git add ${VERSSPHX}-osx-unsigned/${SIGNER}
            git commit -a -m "Add ${VERSSPHX} unsigned sigs for ${SIGNER}"
            popd
        fi
fi

# Verify the build
if [[ $verify = true ]]
then
	# Linux
	pushd ./gitian-builder
	echo ""
	echo "Verifying v${VERSSPHX} Linux"
	echo ""
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSSPHX}-linux ../sphx/contrib/gitian-descriptors/gitian-linux.yml
	# Windows
	echo ""
	echo "Verifying v${VERSSPHX} Windows"
	echo ""
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSSPHX}-win-unsigned ../sphx/contrib/gitian-descriptors/gitian-win.yml
	# Mac OSX
	echo ""
	echo "Verifying v${VERSSPHX} Mac OSX"
	echo ""
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSSPHX}-osx-unsigned ../sphx/contrib/gitian-descriptors/gitian-osx.yml
	# AArch64
	echo ""
	echo "Verifying v${VERSSPHX} AArch64"
	echo ""
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSSPHX}-aarch64 ../sphx/contrib/gitian-descriptors/gitian-aarch64.yml
	# Signed Windows
	echo ""
	echo "Verifying v${VERSSPHX} Signed Windows"
	echo ""
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSSPHX}-osx-signed ../sphx/contrib/gitian-descriptors/gitian-osx-signer.yml
	# Signed Mac OSX
	echo ""
	echo "Verifying v${VERSSPHX} Signed Mac OSX"
	echo ""
	./bin/gverify -v -d ../gitian.sigs/ -r ${VERSSPHX}-osx-signed ../sphx/contrib/gitian-descriptors/gitian-osx-signer.yml
	popd
fi

# Sign binaries
if [[ $sign = true ]]
then

        pushd ./gitian-builder
	# Sign Windows
	if [[ $windows = true ]]
	then
	    echo ""
	    echo "Signing ${VERSSPHX} Windows"
	    echo ""
	    ./bin/gbuild -i --commit signature=${COMMIT} ../sphx/contrib/gitian-descriptors/gitian-win-signer.yml
	    ./bin/gsign -p $signProg --signer $SIGNER --release ${VERSSPHX}-win-signed --destination ../gitian.sigs/ ../sphx/contrib/gitian-descriptors/gitian-win-signer.yml
	    mv build/out/sphx-*win64-setup.exe ../sphx-binaries/${VERSSPHX}
	    mv build/out/sphx-*win32-setup.exe ../sphx-binaries/${VERSSPHX}
	fi
	# Sign Mac OSX
	if [[ $osx = true ]]
	then
	    echo ""
	    echo "Signing ${VERSSPHX} Mac OSX"
	    echo ""
	    ./bin/gbuild -i --commit signature=${COMMIT} ../sphx/contrib/gitian-descriptors/gitian-osx-signer.yml
	    ./bin/gsign -p $signProg --signer $SIGNER --release ${VERSSPHX}-osx-signed --destination ../gitian.sigs/ ../sphx/contrib/gitian-descriptors/gitian-osx-signer.yml
	    mv build/out/sphx-osx-signed.dmg ../sphx-binaries/${VERSSPHX}/sphx-${VERSSPHX}-osx.dmg
	fi
	popd

        if [[ $commitFiles = true ]]
        then
            # Commit Sigs
            pushd gitian.sigs
            echo ""
            echo "Committing ${VERSSPHX} Signed Sigs"
            echo ""
            git add ${VERSSPHX}-win-signed/${SIGNER}
            git add ${VERSSPHX}-osx-signed/${SIGNER}
            git commit -a -m "Add ${VERSSPHX} signed binary sigs for ${SIGNER}"
            popd
        fi
fi
