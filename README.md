# AkiSpiLcd
## get [git-remote-hg](https://github.com/felipec/git-remote-hg)
* `apt-get install sudo apt-get install git-remote-hg`
* *Ubuntu 16.04* has package

## clone and commit
```
git clone hg::https://k4zuki@developer.mbed.org/users/k4zuki/code/AkiSpiLcd/
cd AkiSpiLcd
git pull  #
git push  # those two does not do anything
touch README.md
git add README.md
git commit -m"add README.md"
```

## edit `.git/config`
```
...
[remote "origin"]
	url = hg::https://k4zuki:********@developer.mbed.org/users/k4zuki/code/AkiSpiLcd/
	fetch = +refs/heads/*:refs/remotes/origin/*
...
```

# try a push back to mbed repository
```
git push      # not working
git fetch     # somthing happened
git push      # NG
git merge     # looks nothing
git merge origin/master # looks NG
git fetch     # \
git push      # |
git fetch     #  > one of these made it?
git pull      # /
git push      # worked here

searching for changes
no changes found
searching for changes
remote: adding changesets
remote: adding manifests
remote: adding file changes
remote: added 1 changesets with 1 changes to 1 files
To hg::https://k4zuki:********@developer.mbed.org/users/k4zuki/code/AkiSpiLcd/
   b7cc2d6..3b430ff  master -> master
```
