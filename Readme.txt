1) download and extract boost
2) modify Makefile.am to use proper boost include/lib directories

emeralddb
A Key-value Database System.

Tools
Boost/thread BSON

Envirements
Install Boosts. See offcial document such as tutorial
Install bson. just copy bson-cxx to src //note: in this respority already copyed
Install And Use
git clone this repository
run shell script build.sh to make.
And Then the executable file edb&emeralddb will be made in bin/

run emralddb first
run edb && input connect hostname port
note: in this program connect localhost 12551

input insert {_id:1,key:"value"}
the key-value will be storaged in data file.

input query {_id:1}
the dataSystem will find the key in index and return the record

input delete {_id:1}
the record and index of key will be erased in data file



https://github.com/cycmay/notes/blob/master/DataBase/emeraldDB-implements.md