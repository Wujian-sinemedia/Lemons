GIT_SUBMODULE_UPDATE_COMMAND := git submodule update --init --recursive --merge

GIT_SUBMODULE_UTH_COMMAND := git checkout $(GIT_BRANCH_NAME) && git fetch && git pull && $(GIT_SUBMODULE_UPDATE_COMMAND)

GIT_UTH := git fetch && git pull && $(GIT_SUBMODULE_UPDATE_COMMAND) && git submodule foreach --recursive '$(GIT_SUBMODULE_UTH_COMMAND)'
