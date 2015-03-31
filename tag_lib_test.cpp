#include <stdio.h>
#include <getopt.h>
#include <tag.h>
#include <tpropertymap.h>
#include <tstring.h>
#include <tstringlist.h>
#include <fileref.h>
#include <tfile.h>

using namespace TagLib;

void updatePmap(PropertyMap& pmap, const char * key, const char * val)
{
    String keyString(key);
    String valString(val);
    StringList valStringList(valString);

    pmap.insert(keyString, valStringList);
}

/*
 * PropertyMap::merge appends values to the map element. We need to replace
 * it instead.
 */
PropertyMap replacingMerge(PropertyMap& originMap, PropertyMap newMap)
{
    PropertyMap mergedMap;

    for(PropertyMap::ConstIterator i = originMap.begin(); i != originMap.end(); ++i) {
        if(newMap.contains(i->first)) {
            mergedMap[i->first] = newMap[i->first];
        } else {
            mergedMap[i->first] = i->second;
        }
    }

    return mergedMap;
}

void updateTarget(FileRef &file, const PropertyMap &map)
{
    PropertyMap origin = file.file()->properties();
    PropertyMap mergedMap = replacingMerge(origin, map);
    file.file()->setProperties(mergedMap);
    file.save();
}

int main(int argc, char * argv[])
{
    int c;
    int index = 0;
    PropertyMap pmap;
    static struct option long_options[] =
    {
        {"album"      , required_argument, 0, 'l'},
        {"artist"     , required_argument, 0, 'a'},
        {"albumartist", required_argument, 0, 'b'},
        {"genre"      , required_argument, 0, 'g'},
        {"year"       , required_argument, 0, 'y'},
        {"title"      , required_argument, 0, 't'},
        {"track"      , required_argument, 0, 'n'},
        {0            , 0                , 0,  0 }
    };

    while((c = getopt_long(argc, argv, "l:a:g:y:t:n:", long_options, &index)) != -1)
    {
        switch(c)
        {
            case 'l':
                updatePmap(pmap, "ALBUM", optarg);
                break;
            case 'a':
                updatePmap(pmap, "ARTIST", optarg);
                break;
            case 'b':
                updatePmap(pmap, "ALBUMARTIST", optarg);
                break;
            case 'g':
                updatePmap(pmap, "GENRE", optarg);
                break;
            case 'y':
                updatePmap(pmap, "DATE", optarg);
                break;
            case 't':
                updatePmap(pmap, "TITLE", optarg);
                break;
            case 'n':
                updatePmap(pmap, "TRACK", optarg);
                break;
            default:
                abort();
        }
    }

    for(int i = optind; i < argc; i++)
    {
        FileRef ref(argv[i]);
        updateTarget(ref, pmap);
    }

    return 0;
}
