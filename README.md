Compilation :<br>
windows : <br>
installer wsl puis suivre instructions pour linux :<br>
[tutoriel](https://www.youtube.com/watch?v=74JHjVLMgqU) <br> 
Linux : <br>
installer SDL2 et gcc <br>
```sudo apt update
  sudo apt install sdl2-dev g++ git
  git clone https://github.com/ddezORTEP/pb3d
  cd pb3d
```

  compilation <br>
```
g++ cube.cpp -o cube -lSDL2
./cube
```
installer pbedit (optionnel)
``` 
sudo apt install libncurses-dev
git clone https://github.com/ddezORTEP/ddezedit
cd ddezedit
```
pour version avec support vi : <br>
`` g++ pbedit-vi.cpp -o pbedit -lncurses`` <br>
pour la version sans vi : <br>
``g++ pbedit_linunx.cpp -o pbedit -lncurses `` <br>
usage : <br>
``./pbedit <fichier>``
