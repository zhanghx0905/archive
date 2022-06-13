rm -rf hw1
git init hw1
cd hw1
echo "Initial submission" >> README.md
git add *
git commit -m "Initial submission"
git checkout -b dev
echo "Unstable development branches" >> README.md
git add *
git commit -m "Unstable development branches"
git checkout -b feat
echo "New features" >> README.md
git add *
git commit -m "New features"
git checkout dev
git merge --no-ff feat -m "Merging new features"
git checkout master
git merge --no-ff dev -m "release"

git log --graph --all # 可视化
git bundle create submit.bundle master # 打包