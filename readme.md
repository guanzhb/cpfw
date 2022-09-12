brief
=====
  cross platform framework @see doc/
     
blueprint
=========
  Logic
  -----
    user only needs to care about implements of widgets and data define,
    how to execute is defined in xml, including:
      1. widgets' data.
      2. invoke chain.
      3. when and how to invoke.
      4. whether and how to convert data when invoke.

  SimpleLogic
  -----------
    only has invoke chain compared with above Logic.

  StateMachine
  ------------
    includes state machine and state.
  
  *middlewares in cpfw/base can be used alone.*

pull
====
  git clone --recurse-submodules git@github.com:guanzhb/cpfw.git

build
=====
  build libcpfw.so
  ----------------
    1. go to cpfw/
    2. cmake -G Ninja
    3. ninja

  build example bin
  -----------------
    1. go to exmaples/xx/
    2. cmake -G Ninja
    3. ninja
