#define DEF_DLL_NAME	"sdHook.dll"
#define DEF_HOOKPROC	"HookProc"
#define DEF_UNHOOKPROC	"UnhookProc"
#define DEF_LOADDATA	"LoadData"
#define DEF_RELOADDATA	"ReLoadData"

typedef void (*PFN_HOOKPROC)();
typedef void (*PFN_UNHOOKPROC)();
typedef	BOOLEAN	(*PFN_LOADDATA)(LPSTR lpFileName);
typedef	BOOLEAN	(*PFN_RELOADDATA)();

BOOLEAN	HookProc(void);
BOOLEAN	UnhookProc(void);
BOOLEAN	LoadDataForDll(LPSTR lpFileName);
BOOLEAN ReLoadDataForDll(void);