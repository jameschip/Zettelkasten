# Zettelkasten (zk)
This is an implementation of a zettelkasten (German, slip box) journaling method for Linux. The basic idea of a Zettelkasten is that short ideas, or notes, are made on numbered sips of paper kept in boxes. These notes have the following properties in this implementation:

* A note has a title. 
* A note has a short idea or bit of information on it. 
* A note can have a number of tags associated with it that group them into contexts. 
* A note can also have other notes linked to it by referencing other notes numbers. 
* A link to another note has a context associated with it, this is the reason why these two notes are linked. 

One of the most important parts here is the context given to the links between notes. This context is more useful than just providing a title, it does more than tell you which note is linked; it tells you WHY! This extra context seems insignificant on paper but makes a huge difference in practice.  
  
Assume for instance we have 2 entries in our Zettelkasten with the titles "Ferns" and "Spores". If "Ferns" has a link to "Spores" and just the title of the entries is used then we have little idea of how spores are related to ferns. If however instead we have a link to the "Spores" note with the context "Ferns reproduce using spores" we now know why the notes are related.

## Purposeful limits.
There are limits imposed by design.

* A notes content may only be 500 characters. Notes should be short and punchy, you are not writing an essay per entry. Break longer things into multiple notes. 
* You may never edit a notes content, If you need to provide corrections then create a new note and link them. This will not only teach you about the topic; but also the way you learn. 
* You may never remove a note from the Zettelkasten. There is no point to removing the past. 

## How?
Pull this repository and run the make file. The built binary will be ```/build/zk```. Move this to somewhere in your path and use from the command line. More complete instructions on how to use the software will be available somewhere.

## Where?
The entries in created by zk will be in a folder in ```~/Zettelkasten```. All of the entries will be in an extension less numbed text file counting from 0 upwards by one every time  you create a new entry. 

## What format?
It was important from the offset that the entries this Zettelkasten are in plain text and readable from outside the application itself. This also means that, providing it uses the same format, other software would be able to interact with your entries in a meaningful way.  
Entries have a header section that is used to define things like tags, links to other entries and the title. Anything outside of this header material is considered the content of entry itself.  
Header sections begin and end with the string ```---\n``` and each item is on its own line. The first character of the line indicates what data that line holds.

* '>' The title
* '#' - A tag
* '~' - A link to another entry.

An example file is:  
```  
---
> The title of the note
# tag_has_no_spaces
# another_tag
~ {10} : This links to entry 10 with this context text.
~ {13} : and this links to 13 with this context text.
---
This is the content of the entry!
```
 
## Why?
I built this for me. This is the note taking software I wanted. I share it here in case others may find it useful and so other may learn form (my mistakes in) the code.
