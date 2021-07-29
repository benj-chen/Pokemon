Do not edit the data folder directly. Edit it with bot.py. It assumes that bot.py and data are in the same directory, and that you are running it from that directory.

The p0 folder is a sample folder. All folders will have a p as their first letter to comply with general naming conventions. The sample folder has the sample data on Mew.

The folder's title will be "p" plus its pokedex number, without leading zeroes. For example, the data about the Pokemon with Pokedex ID 1 will have the folder title p1.

Inside the folder will be a plain text file called info, and one or two folders. If there is one folder, that means that there is one gender form, or that gender does not affect the aesthetic. If so, that folder is named "d" for "default". Otherwise, there are two genders, so the folders would be named "m" and "f" in some order, for male and female.
