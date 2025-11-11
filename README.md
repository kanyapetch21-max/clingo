C-LINGO C Project (v3) - Smart Flashcard Edition
================================================

- Topic-based vocabulary learning
- Multiple-choice & typing questions
- Hearts system (lose hearts on wrong answers)
- XP & Level system
- User profiles saved in data/users.txt
- Tracks wrong answers per item (mistakes)
- Flashcard mode:
    - Shows ONLY words with frequent mistakes (mistakes >= 2)

Build (Windows, MinGW):

    gcc -std=c99 -Wall -o clingo main.c lesson.c fileio.c user.c ui.c util.c

Run:

    clingo.exe
