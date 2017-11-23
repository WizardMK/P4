
#ifndef defragmenterH
  #define defragmenterH

#include "mynew.h"
#include "DefragRunner.h"

class Defragmenter
{
public:
  Defragmenter(DiskDrive *diskDrive);

}; // class Defragmenter

#endif


/*
// Author Sean Davis
#ifndef defragmenterH
  #define defragmenterH

#include "mynew.h"
#include "DefragRunner.h"

class Item {
    public:
    int old_location;
    int new_location;
    bool in_arr;

    bool operator==(const Item& other) const {
        return old_location == other.old_location && new_location == other.new_location;
    }

    bool operator!=(const Item& other) const {
        return old_location != other.old_location;
    }
};


class Defragmenter
{
public:
  Defragmenter(DiskDrive *diskDrive);

}; // class Defragmenter

#endif
*/
