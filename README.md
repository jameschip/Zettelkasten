# Zettlekasten

This is an implementation of a zettlekasten (German, slip box) journaling method for Linux.
The basic idea of a Zettlecasten is that short ideas, or notes, are made on numbered sips of paper kept in boxes. These notes have the followingg properties in this implementation:

* A note has a title.
* A note has a short idea or bit of information on it.
* A note can have a number of tags associated with it that group them into contexts.
* A note can also have other notes linked to it by referencing other notes numbers.
* A link to another note has a context associated with it, this is the reason why these two notes are linked.

One of the most important parts here is the context given to the links between notes. This context is more usefull than just providing a title, it does more than tell you which note is linked; it tells you WHY!
This extra context seems insignificant on paper but makes a huge difference in practice.  
  
Assume for instance we have 2 entries in ouur Zettlekasten with the titles "Ferns" and "Spores". If "Ferns" has a link to "Spores" and just the title of the entries is used then we have little idea of how spores
are related to ferns. If however instead we have a link to the "Spores" note with the context "Ferns reproduce using spores" we now know why the notes are related.

## Purposeful limits.

There are limits imposed by design. 

* A notes content may only be 500 characters. Notes should be short and punchy, you are not writing an essay per entry. Break longer things into multiple notes.
* You may never edit a notes content, If you need to provide corrections then create a new note and link them. This will not only teach you about the topic; but also the way you learn.
* You may never remove a note from the Zettlekasten. There is no point to removing the past.

## Why?

I built this for me. This is the note taking software I wanted. I share it here incase others may find it usefull and so other may learn form (my mistakes in) the code.
