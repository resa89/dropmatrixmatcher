# Drop Matrix Macher
This was my Bachelor thesis project. Which was developed at the Media University Stuttgart.

The drop matrix matcher is a software to automatic detect patterns in counterfeits. It was developed for the benefit of the _Kriminaltechnische Untersuchungsstelle Baden-Württemberg_ (the forensic investigation office of Baden-Württemberg). Therefor a template matching algorithm that is invariant in intensity and contrast was implemented.


## GUI development

The GUI was developed with the _QtCreator_ and can be adapted within the file _dropmatrixmatcher.ui_. The funcitoinallity implementation at every GUI element is linked in the _dropmatrixmatcher.cpp_ file.


## Algorithm development

The automatic matching functionality is implemented in the _matching.cpp_ file. Wheres user driven image processing steps (preprocessing like contrast, color values or brightness) are implemented in the file _dropmatrixmatcher.cpp_. 


## DB connection

To setup the database connection the function `Window::setupModel()` in the file _dropmatrixmatcher.cpp_ has to be adapted.


