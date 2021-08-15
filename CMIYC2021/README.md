
# Team Hashcat - Crack me if you can 2021

Collection of stuff the team made during contests




| file | Description                |
| :-------- | :------------------------- |
| `CMIYC2021TeamHashcatWriteup.pdf`| writeup |
| `CMIYC2021-NTDS-extract-by-kontrast23.pdf`| writeup - alternative extraction of hashes from VM |
| `base.rules` | use infront of other rules to cover basic modifiers  |
| `dd.table` | table for simple_table.pl to cover all confuseables  http://www.unicode.org/Public/security/latest/confusables.txt|
| `ferengi.rules` | opens up all history0 with base plains from history1 for the ferengi group  |
| `leet.table` | table for simple_table.pl extented leet to cover all extra modifiers found in cmiyc2021  |
| `simple_table.pl` | standalone implementation of table attack (from hashcat-legacy)  |
| `permute3.c` | Tool version 3 for applying permutation attacks like permute.c from hashcat-utils, but not for individual letters of passwords, but for words  |
| `splitter.pl` | Extract 5/N random words for Bible psalm |
| `minintlm3.c` | Standalone NTLM cracker |
| `history0.c` | A tool developed specifically to streamline the attack chains used in cmiyc for history0 |
| `bla2.sh` | script to run history0 N times |


  
