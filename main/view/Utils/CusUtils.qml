pragma Singleton
import QtQuick 2.0

//Rectangle{
//    height:300;width:300;
//}
Item{
    function hello(){console.log("hello world.");}
    function rgb(r,g,b){var ret=(r<<16|g<<8|b); return ("#"+ret.toString(16).toUpperCase());}
}
