# Pokemon

My attempt to recreate Pokemon Diamond, the first pokemon game I played... on an emulator when I was 14 years old.

Graphics are managed almost exclusively by Pygame. Credits to Pygame for existing.

Data about each pokemon is managed by their pokedex number.

I have a bot that tries to automate the hard parts of entering the data.

### sprites

For each pokemon, there are a couple things that can change the appearance of a sprite:

- gender, ie the Starly evolution line. There are always either one or two genders. I will include this in my code.
- shiny, applicable to all pokemon from gen 2 onward. This happens purely by chance. I will include this.
- form, ie the Shellos line, where West Sea Shellos spawn west of Mt. Coronet, and East Sea Shellos spawn to the east. I will not include this yet.
- region in the later games, but I'm not doing this. I might add it later. Or I might consider it as form. Either way, not including this
- dynamax (daimakkusu) and gigantamax (kyodaimakkusu), but I'm not doing this because it's in a later gen and probably is going to be annoying to code.
- the weather, i.e. castform, 

Exceptions:

- I am not including Rotom into the game. I might hard-code this into the game but this is too difficult without making the rest of the data a nuisance to deal with.
- I am not including 