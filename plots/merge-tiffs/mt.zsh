#!/bin/zsh -eu

cd $( dirname $0 )

paste {workers,rate}.data > merge-tiffs.data

swift_plotter.zsh -s merge-tiffs.{cfg,eps,data}
