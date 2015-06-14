#include "leaks.h"
/* Leak Check Code */
  _CrtSetDbgFlag
   ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  _CrtSetReportMode
   ( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
  /* End Leak Check Code */
