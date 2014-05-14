#!/bin/zsh

# From Yadu on swift-devel 4/23/2014
# We are not going to use this- we don't need Coasters

set -eu
set -x

PORT1=51001
PORT2=51005

REMOTE_HOST=cetus.alcf.anl.gov
REMOTE_USER=wozniak
REMOTE=${REMOTE_USER}@${REMOTE_HOST}
REMOTE_WORKER_DIRECTORY=/home/${REMOTE_USER}

ssh -L ${PORT1}:${REMOTE_HOST}:${PORT1} ${REMOTE} & 
SSH_TUNNEL_PID=${!}
declare SSH_TUNNEL_PID

sleep 1

coaster-service -p ${PORT1} -localport ${PORT2} -nosec -passive &> coaster-service.log &

REMOTE_WORKER=${REMOTE_WORKER_DIRECTORY}/worker.pl
REMOTE_LOG=${REMOTE_WORKER_DIRECTORY}/worker.log

SWIFT=$( which swift )
SWIFT_BIN=${SWIFT:h}

ssh ${REMOTE} mkdir -p ${REMOTE}/${REMOTE_WORKER_DIRECTORY}
scp ${SWIFT_BIN}/worker.pl ${REMOTE}:${REMOTE_WORKER_DIRECTORY}
ssh ${REMOTE} ${REMOTE_WORKER} http://localhost:${PORT2} 0099 ${REMOTE_LOG} &
