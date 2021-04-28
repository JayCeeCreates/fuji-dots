#! /bin/bash

if [[ $EUID -ne 0 ]]; then

	echo "Installing rice dependencies..."

	sleep 3

	sudo pacman -S xorg xorg-server picom feh dunst pcmanfm ttf-font-awesome firefox lxappearance neofetch xarchiver zsh zsh-autosuggestions zsh-syntax-highlighting git --noconfirm --needed

	git clone --depth=1 https://github.com/romkatv/powerlevel10k.git $HOME/.p10k

	echo "Building suckless components..."

	sleep 3

	cd ./dwm
	./build
	cd ../st
	./build
	cd ../dmenu
	./build
	cd ../slstatus
	./build
	cd ../slock
	./build
	cd ..

	echo "Copying files"

	sleep 3

	mkdir -p $HOME/Pictures
	mkdir -p $HOME/.cache

	sudo cp -v "other files/monogram_extended.ttf" /usr/share/fonts/
	sudo cp -v "other files/slock@.service" /etc/systemd/system/

	cp -rv DWM/{.config,.icons,.themes,"other files/"{.fehbg,.p10k.zsh,.xinitrc,.zshrc}} $HOME/
	cp -v "other files/bg.jpg" $HOME/Pictures/

	echo "Finishing touches..."

	sleep 3

	touch $HOME/.cache/zshhistory

	sudo systemctl enable slock@$USER.service

	fc-cache

	chsh -s /bin/zsh

	echo "All done! Type in 'startx' to start and make some configurations like changing the GTK theme and icon theme through 'lxappearance'."

	exit 1

else
	echo "Running in root. Ignoring."
	exit 1
fi
