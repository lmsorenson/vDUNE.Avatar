# vDUNE.Touch

## Step 1
git remote add -f vDUNE_Avatar https://github.com/lmsorenson/vDUNE_Avatar.git
git subtree add --prefix vDUNE_Avatar master --squash
git subtree push --prefix vDUNE_Avatar master
git subtree pull --prefix=vDUNE_Avatar 
