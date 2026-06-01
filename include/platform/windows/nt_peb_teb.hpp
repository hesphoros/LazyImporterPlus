//
// @file nt_peb_teb.hpp
// @fileinfo Windows NT PEB and TEB structures
// @author hesphoros
// @date 2024/06/01
// @see 
//    https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb
//    https://ntdoc.m417z.com/peb
//    https://github.com/winsiderss/phnt
//    

#ifndef __NT_PEB_TEB_HPP__
#define __NT_PEB_TEB_HPP__

#ifndef _LARGE_INTEGER_DEFINED
#define _LARGE_INTEGER_DEFINED
#include <cstdint>
#include <cstddef>

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field) offsetof(type, field)
#endif
/**
 * The LARGE_INTEGER union defines a 64-bit signed integer that can be accessed as either 
 * a whole 64-bit value (QuadPart) or as two separate 32-bit values (LowPart and HighPart).
 * \sa https://learn.microsoft.com/zh-cn/windows/win32/api/winnt/ns-winnt-large_integer-r1
 */
typedef union _LARGE_INTEGER {
    struct {
        unsigned long    LowPart;
        long            HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        unsigned long    LowPart;
        long            HighPart;
    } u;
    long long QuadPart;
} LARGE_INTEGER,*PLARGE_INTEGER;

/**
 * The ULARGE_INTEGER union defines a 64-bit unsigned integer that can be accessed as either 
 * a whole 64-bit value (QuadPart) or as two separate 32-bit values
 * 
 * \sa https://learn.microsoft.com/zh-cn/windows/win32/api/winnt/ns-winnt-ularge_integer-r1
 */
typedef union _ULARGE_INTEGER {
    struct {
        unsigned long    LowPart;
        unsigned long    HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        unsigned long    LowPart;
        unsigned long    HighPart;
    } u;
    unsigned long long QuadPart;
} ULARGE_INTEGER,*PULARGE_INTEGER;
#endif // _LARGE_INTEGER_DEFINED

/**
 * The UNICODE_STRING structure defines a counted string used for Unicode strings.
 * Use RtlUnicodeStringInit or `RtlUnicodeStringInitEx` to initialize a UNICODE_STRING structure.
 * The MaximumLength is used to indicate the length of Buffer so that if the string is passed
 *  to a conversion routine such as `RtlAnsiStringToUnicodeString` the returned string does
 *  not exceed the buffer size.
 * \sa https://learn.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_unicode_string
 */
typedef struct _UNICODE_STRING
{
    unsigned short        Length;  ///< The length, in bytes, of the string stored in Buffer.
    unsigned short MaximumLength;  ///< The length, in bytes, of Buffer.
    wchar_t*              Buffer;  ///< Pointer to a buffer used to contain a string of wide characters.
} UNICODE_STRING, *PUNICODE_STRING;


typedef struct _STRING
{
    unsigned short        Length;
    unsigned short MaximumLength;
    char*                 Buffer;
} STRING, *PSTRING, ANSI_STRING, *PANSI_STRING, OEM_STRING, *POEM_STRING;
/**
 * The LIST_ENTRY structure defines a doubly linked list.
 *  It is used by the PEB_LDR_DATA structure to maintain lists of loaded modules.
 */
typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;  ///< Pointer to the next entry in the list.
    struct _LIST_ENTRY *Blink;  ///< Pointer to the previous entry in the list.
} LIST_ENTRY,*PLIST_ENTRY,*PRLIST_ENTRY;


#define DECLSPEC_ALIGN(x) __declspec(align(x))
#if defined(_WIN64)
typedef struct DECLSPEC_ALIGN(16) _SLIST_ENTRY
{
    struct _SLIST_ENTRY* Next;
} SLIST_ENTRY, *PSLIST_ENTRY;

typedef union DECLSPEC_ALIGN(16) _SLIST_HEADER
{
    struct
    {
        unsigned long long Alignment;
        unsigned long long Region;
    };

    struct
    {
        unsigned long long Depth      : 16;
        unsigned long long Sequence   : 9;
        unsigned long long NextEntry  : 39;
        unsigned long long HeaderType : 1;
        unsigned long long Init       : 1;
        unsigned long long Reserved   : 59;
        unsigned long long Region     : 3;
    } Header8;

    struct
    {
        unsigned long long Depth      : 16;
        unsigned long long Sequence   : 48;
        unsigned long long HeaderType : 1;
        unsigned long long Reserved   : 3;
        unsigned long long NextEntry  : 60;
    } HeaderX64;

} SLIST_HEADER, *PSLIST_HEADER;

#else
typedef struct _SLIST_ENTRY
{
    struct _SLIST_ENTRY* Next;
} SLIST_ENTRY, *PSLIST_ENTRY;
typedef union _SLIST_HEADER
{
    unsigned long long Alignment;

    struct
    {
        SLIST_ENTRY Next;
        unsigned short Depth;
        unsigned short Sequence;
    };

} SLIST_HEADER, *PSLIST_HEADER;
#endif // _WIN64

typedef struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY *Next;
}SINGLE_LIST_ENTRY,*PSINGLE_LIST_ENTRY;
/**
 * The PEB_LDR_DATA structure contains information about the loaded modules for the process.
 * \sa https://learn.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb_ldr_data
 * @param InMemoryOrderModuleList 
 * * The head of a doubly-linked list that contains the loaded modules for the process. 
 * * Each item in the list is a pointer to an LDR_DATA_TABLE_ENTRY structure.
 */
typedef struct _PEB_LDR_DATA
{
    unsigned long                             Length;
    unsigned char                        Initialized;
    void*                                   SsHandle;
    LIST_ENTRY                 InLoadOrderModuleList;
    LIST_ENTRY               InMemoryOrderModuleList;
    LIST_ENTRY       InInitializationOrderModuleList;
    void*                            EntryInProgress;
    unsigned char                 ShutdownInProgress;
    void*                           ShutdownThreadId;
} PEB_LDR_DATA, *PPEB_LDR_DATA;


/**
 * A record for a service tag associated with a loader module.
 */
typedef struct _LDR_SERVICE_TAG_RECORD
{
    struct _LDR_SERVICE_TAG_RECORD      *Next;  ///< Pointer to the next service tag record in the list.
    unsigned long                  ServiceTag;  ///< The service tag value associated with the loader module.
} LDR_SERVICE_TAG_RECORD, *PLDR_SERVICE_TAG_RECORD;



/**
 * A circular singly linked list used by the loader.
 */
typedef struct _LDRP_CSLIST
{
    PSINGLE_LIST_ENTRY Tail;
} LDRP_CSLIST, *PLDRP_CSLIST;


/**
 * The state of a Directed Dependency Acyclic Graph (DDAG) node.
 */
typedef enum _LDR_DDAG_STATE
{
    LdrModulesMerged                    = -5,
    LdrModulesInitError                 = -4,
    LdrModulesSnapError                 = -3,
    LdrModulesUnloaded                  = -2,
    LdrModulesUnloading                 = -1,
    LdrModulesPlaceHolder               = 0,
    LdrModulesMapping                   = 1,
    LdrModulesMapped                    = 2,
    LdrModulesWaitingForDependencies    = 3,
    LdrModulesSnapping                  = 4,
    LdrModulesSnapped                   = 5,
    LdrModulesCondensed                 = 6,
    LdrModulesReadyToInit               = 7,
    LdrModulesInitializing              = 8,
    LdrModulesReadyToRun                = 9
} LDR_DDAG_STATE;


/**
 * A node in the Directed Dependency Acyclic Graph (DDAG) used by the loader to manage module dependencies.
 */
typedef struct _LDR_DDAG_NODE
{
    LIST_ENTRY Modules;
    PLDR_SERVICE_TAG_RECORD ServiceTagList;
    unsigned long LoadCount;
    unsigned long LoadWhileUnloadingCount; // ReferenceCount before WIN10
    unsigned long LowestLink; // DependencyCount before WIN10
    union
    {
        LDRP_CSLIST Dependencies;
        SINGLE_LIST_ENTRY RemovalLink;
    };
    LDRP_CSLIST IncomingDependencies;
    LDR_DDAG_STATE State;
    SINGLE_LIST_ENTRY CondenseLink;
    unsigned long PreorderNumber;
} LDR_DDAG_NODE, *PLDR_DDAG_NODE;



/**
 * A structure representing a dependency between loader modules.
 */
typedef struct _LDRP_DEPENDENCY
{
    SINGLE_LIST_ENTRY Link;
    PLDR_DDAG_NODE ChildNode;
    SINGLE_LIST_ENTRY BackLink;
    union
    {
        PLDR_DDAG_NODE ParentNode;
        struct
        {
            unsigned long ForwarderLink : 1;
            unsigned long SpareFlags : 2;
        };
    };
} LDRP_DEPENDENCY, *PLDR_DEPENDENCY;


// HotPatchState
/**
 * The hot-patching state of a loader module.
 */
typedef enum _LDR_HOT_PATCH_STATE
{
    LdrHotPatchBaseImage,
    LdrHotPatchNotApplied,
    LdrHotPatchAppliedReverse,
    LdrHotPatchAppliedForward,
    LdrHotPatchFailedToPatch,
    LdrHotPatchStateMax,
} LDR_HOT_PATCH_STATE, *PLDR_HOT_PATCH_STATE;

/**
 * The reason a DLL was loaded.
 */
typedef enum _LDR_DLL_LOAD_REASON
{
    LoadReasonUnknown                       = -1,
    LoadReasonStaticDependency              = 0,
    LoadReasonStaticForwarderDependency     = 1,
    LoadReasonDynamicForwarderDependency    = 2,
    LoadReasonDelayloadDependency           = 3,
    LoadReasonDynamicLoad                   = 4,
    LoadReasonAsImageLoad                   = 5,
    LoadReasonAsDataLoad                    = 6,
    LoadReasonEnclavePrimary                = 7, // since REDSTONE3
    LoadReasonEnclaveDependency             = 8,
    LoadReasonPatchImage                    = 9, // since WIN11
} LDR_DLL_LOAD_REASON, *PLDR_DLL_LOAD_REASON;


typedef struct _ACTIVATION_CONTEXT ACTIVATION_CONTEXT, *PACTIVATION_CONTEXT;
typedef struct _LDRP_LOAD_CONTEXT *PLDRP_LOAD_CONTEXT;
typedef struct _ACTIVATION_CONTEXT_DATA ACTIVATION_CONTEXT_DATA, *PACTIVATION_CONTEXT_DATA;


typedef struct _ASSEMBLY_STORAGE_MAP_ENTRY
{
    unsigned long                     Flags;
    UNICODE_STRING                  DosPath;
    void*                            Handle;
} ASSEMBLY_STORAGE_MAP_ENTRY, *PASSEMBLY_STORAGE_MAP_ENTRY;

#define ASSEMBLY_STORAGE_MAP_ASSEMBLY_ARRAY_IS_HEAP_ALLOCATED 0x00000001

typedef struct _ASSEMBLY_STORAGE_MAP
{
    unsigned long                       Flags;
    unsigned long                AssemblyCount;
    PASSEMBLY_STORAGE_MAP_ENTRY *AssemblyArray;
} ASSEMBLY_STORAGE_MAP, *PASSEMBLY_STORAGE_MAP;


#define ACTIVATION_CONTEXT_NOTIFICATION_DESTROY 1
#define ACTIVATION_CONTEXT_NOTIFICATION_ZOMBIFY 2
#define ACTIVATION_CONTEXT_NOTIFICATION_USED 3
/**
 * @brief Activation context notification callback.
 *
 * Invoked by the activation context subsystem when an activation
 * context is created, referenced, activated, deactivated, or destroyed.
 *
 * @param NotificationType The notification type.
 * * `ACTIVATION_CONTEXT_NOTIFICATION_DESTROY`
 * * `ACTIVATION_CONTEXT_NOTIFICATION_ZOMBIFY`
 * * `ACTIVATION_CONTEXT_NOTIFICATION_USED`
 * @param ActivationContext The activation context being notified.
 * @param NotificationContext Optional notification context.
 * @param NotificationData Optional notification-specific data.
 * @param Context Caller-supplied context pointer.
 */
typedef void ACTIVATION_CONTEXT_NOTIFY_ROUTINE(
    unsigned long               NotificationReason, 
    PACTIVATION_CONTEXT          ActivationContext,
    PACTIVATION_CONTEXT_DATA ActivationContextData,
    void*                      NotificationContext,
    void*                         NotificationData,
    unsigned char*         DisableThisNotification
);
typedef ACTIVATION_CONTEXT_NOTIFY_ROUTINE* PACTIVATION_CONTEXT_NOTIFY_ROUTINE;

/**
 * @brief Represents an activation context object.
 *
 * An activation context contains Side-by-Side (SxS) assembly metadata,
 * manifest information, notification settings, and assembly storage
 * mappings used by the Fusion loader.
 */
typedef struct _ACTIVATION_CONTEXT
{
    long                                           RefCount;  ///< Reference count for the activation context.
    unsigned long                                     Flags;  ///< Activation context flags.
    PACTIVATION_CONTEXT_DATA          ActivationContextData;  ///< Pointer to the activation context data image.
    PACTIVATION_CONTEXT_NOTIFY_ROUTINE  NotificationRoutine;  ///< Optional notification callback routine.
    void*                               NotificationContext;  ///< User-defined context passed to the notification routine.
    unsigned long                      SentNotifications[8];  ///< Bitmap of notifications that have already been sent.
    unsigned long                  DisabledNotifications[8];  ///< Bitmap of notifications that are disabled.
    ASSEMBLY_STORAGE_MAP                         StorageMap;  ///< Inline storage map entries used for assembly resolution.
    PASSEMBLY_STORAGE_MAP_ENTRY InlineStorageMapEntries[32];
} ACTIVATION_CONTEXT, *PACTIVATION_CONTEXT;




// Windows Fusion（SxS，Side-by-Side Assembly）
// (const struct _ACTIVATION_CONTEXT_DATA *)PEB->ActivationContextData 
// @see https://learn.microsoft.com/en-us/windows/win32/sbscs/activation-context-reference
typedef struct _ACTIVATION_CONTEXT_DATA
{
    unsigned long                    Magic; ///< 0x78746341 ('Actx' in little-endian) Activation Context
    unsigned long               HeaderSize; ///< The size of the header, including all the variable-length fields.
    unsigned long            FormatVersion; ///< The format version of the activation context data. The current version is 1.
    unsigned long                TotalSize; ///< The total size of the activation context data, including the header and all the variable-length fields.
    unsigned long         DefaultTocOffset; ///< to ACTIVATION_CONTEXT_DATA_TOC_HEADER
    unsigned long        ExtendedTocOffset; ///< to ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADER
    unsigned long     AssemblyRosterOffset; ///< to ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER
    unsigned long                    Flags; ///< ACTIVATION_CONTEXT_FLAG_*
} ACTIVATION_CONTEXT_DATA, *PACTIVATION_CONTEXT_DATA;

//
// Balanced tree node
//

#ifndef RTL_BALANCED_NODE_RESERVED_PARENT_MASK
#define RTL_BALANCED_NODE_RESERVED_PARENT_MASK 3
#endif

typedef struct _RTL_BALANCED_NODE
{
    union
    {
        struct _RTL_BALANCED_NODE *Children[2];
        struct
        {
            struct _RTL_BALANCED_NODE *Left;
            struct _RTL_BALANCED_NODE *Right;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
    union
    {
        unsigned char Red : 1;
        unsigned char Balance : 2;
        uintptr_t ParentValue;
    } DUMMYUNIONNAME2;
} RTL_BALANCED_NODE, *PRTL_BALANCED_NODE;


#ifndef RTL_BALANCED_NODE_GET_PARENT_POINTER
#define RTL_BALANCED_NODE_GET_PARENT_POINTER(Node) \
    ((PRTL_BALANCED_NODE)((Node)->ParentValue & ~RTL_BALANCED_NODE_RESERVED_PARENT_MASK))
#endif



// LDR_DATA_TABLE_ENTRY->Flags
#define LDRP_PACKAGED_BINARY                    0x00000001
#define LDRP_MARKED_FOR_REMOVAL                 0x00000002
#define LDRP_IMAGE_DLL                          0x00000004
#define LDRP_LOAD_NOTIFICATIONS_SENT            0x00000008
#define LDRP_TELEMETRY_ENTRY_PROCESSED          0x00000010
#define LDRP_PROCESS_STATIC_IMPORT              0x00000020
#define LDRP_IN_LEGACY_LISTS                    0x00000040
#define LDRP_IN_INDEXES                         0x00000080
#define LDRP_SHIM_DLL                           0x00000100
#define LDRP_IN_EXCEPTION_TABLE                 0x00000200
#define LDRP_VERIFIER_PROVIDER                  0x00000400 // reserved before WIN11 24H2
#define LDRP_SHIM_ENGINE_CALLOUT_SENT           0x00000800 // reserved before WIN11 24H2
#define LDRP_LOAD_IN_PROGRESS                   0x00001000
#define LDRP_LOAD_CONFIG_PROCESSED              0x00002000 // reserved before WIN10
#define LDRP_ENTRY_PROCESSED                    0x00004000
#define LDRP_PROTECT_DELAY_LOAD                 0x00008000 // reserved before WINBLUE
#define LDRP_AUX_IAT_COPY_PRIVATE               0x00010000 // reserved before WIN11 24H2
#define LDRP_DONT_CALL_FOR_THREADS              0x00040000
#define LDRP_PROCESS_ATTACH_CALLED              0x00080000
#define LDRP_PROCESS_ATTACH_FAILED              0x00100000
#define LDRP_SCP_IN_EXCEPTION_TABLE             0x00200000 // LDRP_COR_DEFERRED_VALIDATE before WIN11 24H2
#define LDRP_COR_IMAGE                          0x00400000
#define LDRP_DONT_RELOCATE                      0x00800000
#define LDRP_COR_IL_ONLY                        0x01000000
#define LDRP_CHPE_IMAGE                         0x02000000 // reserved before REDSTONE4
#define LDRP_CHPE_EMULATOR_IMAGE                0x04000000 // reserved before WIN11
#define LDRP_REDIRECTED                         0x10000000
#define LDRP_COMPAT_DATABASE_PROCESSED          0x80000000

// LDR_DATA_TABLE_ENTRY->ImplicitPathOptions
#define LDR_PATH_IS_FLAGS                              0x00000001  // Indicates the path string is actually a set of flags.
#define LDR_PATH_WITH_ALTERED_SEARCH_PATH              0x00000008  // Corresponds to LOAD_WITH_ALTERED_SEARCH_PATH.
#define LDR_PATH_SEARCH_DLL_LOAD_DIR                   0x00000100  // Corresponds to LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR.
#define LDR_PATH_SEARCH_APPLICATION_DIR                0x00000200  // Corresponds to LOAD_LIBRARY_SEARCH_APPLICATION_DIR.
#define LDR_PATH_SEARCH_USER_DIRS                      0x00000400  // Corresponds to LOAD_LIBRARY_SEARCH_USER_DIRS.
#define LDR_PATH_SEARCH_SYSTEM32                       0x00000800  // Corresponds to LOAD_LIBRARY_SEARCH_SYSTEM32.
#define LDR_PATH_SEARCH_DEFAULT_DIRS                   0x00001000  // Corresponds to LOAD_LIBRARY_SEARCH_DEFAULT_DIRS.
#define LDR_PATH_SAFE_CURRENT_DIRS                     0x00002000  // Corresponds to LOAD_LIBRARY_SAFE_CURRENT_DIRS.
#define LDR_PATH_SEARCH_SYSTEM32_NO_FORWARDER          0x00004000  // Corresponds to LOAD_LIBRARY_SEARCH_SYSTEM32_NO_FORWARDER.

// LDR_DATA_TABLE_ENTRY->DependentLoadFlags
#define LDR_LOAD_DONT_RESOLVE_DLL_REFERENCES           0x00000001 // Do not resolve dependencies. (LDR_DONT_RESOLVE_DLL_REFERENCES)
#define LDR_LOAD_LIBRARY_AS_DATAFILE                   0x00000002 // Load as a data file.
#define LDR_LOAD_PACKAGED_LIBRARY                      0x00000004 // Load from a Windows Store package.
#define LDR_LOAD_WITH_ALTERED_SEARCH_PATH              0x00000008 // Use altered search path logic.
#define LDR_LOAD_IGNORE_CODE_AUTHZ_LEVEL               0x00000010 // Ignore code authorization levels.
#define LDR_LOAD_LIBRARY_AS_IMAGE_RESOURCE             0x00000020 // Load as an image resource.
#define LDR_LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE         0x00000040 // Exclusive data file access.
#define LDR_LOAD_LIBRARY_REQUIRE_SIGNED_TARGET         0x00000080 // Require a signed target.
#define LDR_LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR           0x00000100 // Search the DLL's load directory.
#define LDR_LOAD_LIBRARY_SEARCH_APPLICATION_DIR        0x00000200 // Search the application directory.
#define LDR_LOAD_LIBRARY_SEARCH_USER_DIRS              0x00000400 // Search user-specified directories.
#define LDR_LOAD_LIBRARY_SEARCH_SYSTEM32               0x00000800 // Search the System32 directory.
#define LDR_LOAD_LIBRARY_SEARCH_DEFAULT_DIRS           0x00001000 // Search all default directories.
#define LDR_LOAD_LIBRARY_SAFE_CURRENT_DIRS             0x00002000 // Use safe current directory search logic.
#define LDR_LOAD_LIBRARY_SEARCH_SYSTEM32_NO_FORWARDER  0x00004000 // Search System32 without following forwarders.
#define LDR_LOAD_LIBRARY_OS_INTEGRITY_CONTINUITY       0x00008000 // Enforce OS integrity continuity.





/**
 * The LDR_DATA_TABLE_ENTRY structure contains information about a loaded module.
 * \sa https://learn.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb_ldr_data
 * \sa https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/api/ntldr/ldr_data_table_entry.htm
 */
typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY                      InLoadOrderLinks;
    LIST_ENTRY                    InMemoryOrderLinks;
    LIST_ENTRY            InInitializationOrderLinks;
    void*                                    DllBase;
    void*                                 EntryPoint; // PDLL_INIT_ROUTINE
    unsigned long                        SizeOfImage;
    UNICODE_STRING                       FullDllName;
    UNICODE_STRING                       BaseDllName;
    union
    {
        unsigned char FlagGroup[4];
        unsigned long Flags;
        struct
        {
            unsigned long PackagedBinary : 1;
            unsigned long MarkedForRemoval : 1;
            unsigned long ImageDll : 1;
            unsigned long LoadNotificationsSent : 1;
            unsigned long TelemetryEntryProcessed : 1;
            unsigned long ProcessStaticImport : 1;
            unsigned long InLegacyLists : 1;
            unsigned long InIndexes : 1;
            unsigned long ShimDll : 1;
            unsigned long InExceptionTable : 1;
            unsigned long VerifierProvider : 1; // 24H2
            unsigned long ShimEngineCalloutSent : 1; // 24H2
            unsigned long LoadInProgress : 1;
            unsigned long LoadConfigProcessed : 1; // WIN10
            unsigned long EntryProcessed : 1;
            unsigned long ProtectDelayLoad : 1; // WINBLUE
            unsigned long AuxIatCopyPrivate : 1; // 24H2
            unsigned long ReservedFlags3 : 1;
            unsigned long DontCallForThreads : 1;
            unsigned long ProcessAttachCalled : 1;
            unsigned long ProcessAttachFailed : 1;
            unsigned long ScpInExceptionTable : 1; // CorDeferredValidate before 24H2
            unsigned long CorImage : 1;
            unsigned long DontRelocate : 1;
            unsigned long CorILOnly : 1;
            unsigned long ChpeImage : 1; // RS4
            unsigned long ChpeEmulatorImage : 1; // WIN11
            unsigned long ReservedFlags5 : 1;
            unsigned long Redirected : 1;
            unsigned long ReservedFlags6 : 2;
            unsigned long CompatDatabaseProcessed : 1;
        };
    };
    unsigned short                          ObsoleteLoadCount;
    unsigned short                                   TlsIndex;
    LIST_ENTRY                                      HashLinks;
    unsigned long                               TimeDateStamp;
    PACTIVATION_CONTEXT           EntryPointActivationContext;
    void*                                                Lock; // RtlAcquireSRWLockExclusive
    PLDR_DDAG_NODE                                   DdagNode;
    LIST_ENTRY                                 NodeModuleLink;
    PLDRP_LOAD_CONTEXT                            LoadContext;
    void*                                       ParentDllBase;
    void*                                   SwitchBackContext;
    RTL_BALANCED_NODE                    BaseAddressIndexNode;
    RTL_BALANCED_NODE                    MappingInfoIndexNode;
    void*                                        OriginalBase;
    LARGE_INTEGER                                    LoadTime;
    unsigned long                           BaseNameHashValue;
    LDR_DLL_LOAD_REASON                            LoadReason;
    unsigned long                         ImplicitPathOptions; // since WINBLUE
    unsigned long                              ReferenceCount; // since WIN10
    unsigned long                          DependentLoadFlags; // since RS1
    unsigned char                                SigningLevel; // since RS2
    unsigned long                                    CheckSum; // since WIN11
    void*                                ActivePatchImageBase;
    LDR_HOT_PATCH_STATE                         HotPatchState;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

// sizeof(LDR_DATA_TABLE_ENTRY)
#define LDR_DATA_TABLE_ENTRY_SIZE_WINXP FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, DdagNode)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN7 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, BaseNameHashValue)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN8 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, ImplicitPathOptions)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN10 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, SigningLevel)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN11 sizeof(LDR_DATA_TABLE_ENTRY)

// typedef struct PRTL_USER_PROCESS_PARAMETERS{};

// typedef struct _PHEAP{}PHEAP;

// typedef struct _PRTL_CRITICAL_SECTION{}PRTL_CRITICAL_SECTION;

// CURDIR Handle | Flags
#define RTL_USER_PROC_CURDIR_CLOSE 0x00000002
#define RTL_USER_PROC_CURDIR_INHERIT 0x00000003

typedef struct _CURDIR
{
    UNICODE_STRING DosPath;
    void* Handle;
} CURDIR, *PCURDIR;

// RTL_DRIVE_LETTER_CURDIR Flags
#define RTL_MAX_DRIVE_LETTERS 32
#define RTL_DRIVE_LETTER_VALID (unsigned short)0x0001

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
    unsigned short Flags;
    unsigned short Length;
    unsigned long TimeStamp;
    STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;


#define RTL_USER_PROC_DETACHED_PROCESS ((HANDLE)(LONG_PTR)-1)
#define RTL_USER_PROC_CREATE_NEW_CONSOLE ((HANDLE)(LONG_PTR)-2)
#define RTL_USER_PROC_CREATE_NO_WINDOW ((HANDLE)(LONG_PTR)-3)
typedef struct _RTL_USER_PROCESS_PARAMETERS *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
    unsigned long MaximumLength;
    unsigned long Length;

    unsigned long Flags; // RTL_USER_PROC_FLAGS
    unsigned long DebugFlags; // RTL_USER_DEBUG_FLAGS

    void* ConsoleHandle;
    unsigned long ConsoleFlags; // RTL_USER_PROC_CONSOLE_FLAGS
    void* StandardInput;
    void* StandardOutput;
    void* StandardError;

    CURDIR CurrentDirectory;
    UNICODE_STRING DllPath;
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
    void* Environment;

    unsigned long StartingX;
    unsigned long StartingY;
    unsigned long CountX;
    unsigned long CountY;
    unsigned long CountCharsX;
    unsigned long CountCharsY;
    unsigned long FillAttribute;

    unsigned long WindowFlags; // RTL_USER_PROC_WINDOW_FLAGS
    unsigned long ShowWindowFlags;
    UNICODE_STRING WindowTitle;
    UNICODE_STRING DesktopInfo;
    UNICODE_STRING ShellInfo;
    UNICODE_STRING RuntimeData;
    RTL_DRIVE_LETTER_CURDIR CurrentDirectories[RTL_MAX_DRIVE_LETTERS];

    uintptr_t  EnvironmentSize;
    uintptr_t  EnvironmentVersion;

    void* PackageDependencyData;
    unsigned long ProcessGroupId;
    unsigned long LoaderThreads; // THRESHOLD
    UNICODE_STRING RedirectionDllName; // REDSTONE5
    UNICODE_STRING HeapPartitionName; // 19H1
    unsigned long long* DefaultThreadpoolCpuSetMasks;
    unsigned long DefaultThreadpoolCpuSetMaskCount;
    unsigned long DefaultThreadpoolThreadMaximum; // 20H1
    unsigned long HeapMemoryTypeMask; // WIN11 22H2
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;


// RTL_USER_PROCESS_PARAMETERS Flags
#define RTL_USER_PROC_PARAMS_NORMALIZED                 0x00000001 // Pointer representation: 1=absolute pointers, 0=relative offsets; set by RtlNormalizeProcessParams, cleared by RtlDeNormalizeProcessParams
#define RTL_USER_PROC_PROFILE_USER                      0x00000002 // User-mode profiling enabled
#define RTL_USER_PROC_PROFILE_KERNEL                    0x00000004 // Kernel-mode profiling enabled
#define RTL_USER_PROC_PROFILE_SERVER                    0x00000008 // Server-mode profiling enabled
//#define RTL_USER_PROC_RESERVE_64K                     0x00000010 // Unused/reserved
#define RTL_USER_PROC_RESERVE_1MB                       0x00000020 // Reserve 1MB virtual memory (mutually exclusive group)
#define RTL_USER_PROC_RESERVE_16MB                      0x00000040 // Reserve 16MB virtual memory (mutually exclusive group)
#define RTL_USER_PROC_CASE_SENSITIVE                    0x00000080 // Enable case-sensitive filename matching (NTFS)
#define RTL_USER_PROC_DISABLE_HEAP_DECOMMIT             0x00000100 // Disable heap decommitment
#define RTL_USER_PROC_DLL_REDIRECTION_LOCAL             0x00001000 // Enable local DLL redirection (.local manifest behavior)
#define RTL_USER_PROC_APP_MANIFEST_PRESENT              0x00002000 // Application manifest is present
#define RTL_USER_PROC_IMAGE_KEY_MISSING                 0x00004000 // Process registry image key not found (masked during kernel flag validation)
#define RTL_USER_PROC_DEV_OVERRIDE_ENABLED              0x00008000 // Developer override configuration enabled
#define RTL_USER_PROC_OPTIN_PROCESS                     0x00020000 // Process opted into mitigations (Windows 8+)
#define RTL_USER_PROC_SESSION_OWNER                     0x00040000 // Process is session owner
#define RTL_USER_PROC_HANDLE_USER_CALLBACK_EXCEPTIONS   0x00080000 // Handle user callback exceptions
#define RTL_USER_PROC_PROTECTED_PROCESS                 0x00400000 // Process is protected (Windows Vista+)
#define RTL_USER_PROC_RESERVE_PLACEHOLDER               0x01000000 // Reserved user-mapping placeholder mode; grouped with RTL_USER_PROC_RESERVE* flags // PspSetupReservedUserMappings
#define RTL_USER_PROC_SECURE_PROCESS                    0x80000000 // Process is secure (Windows 11+); rejected by current kernel capture validation path


#define GDI_HANDLE_BUFFER_SIZE32 34
#define GDI_HANDLE_BUFFER_SIZE64 60
#ifndef _WIN64
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE32
#else
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE64
#endif

typedef unsigned long GDI_HANDLE_BUFFER[GDI_HANDLE_BUFFER_SIZE];
typedef unsigned long GDI_HANDLE_BUFFER32[GDI_HANDLE_BUFFER_SIZE32];
typedef unsigned long GDI_HANDLE_BUFFER64[GDI_HANDLE_BUFFER_SIZE64];

typedef struct _KERNEL_CALLBACK_TABLE KERNEL_CALLBACK_TABLE, *PKERNEL_CALLBACK_TABLE;

typedef long  (__stdcall  FN_DISPATCH)(
    void* Context
);
typedef FN_DISPATCH* PFN_DISPATCH;
// Peb!KernelCallbackTable = user32.dll!apfnDispatch
typedef struct _KERNEL_CALLBACK_TABLE
{
    PFN_DISPATCH __fnCOPYDATA;
    PFN_DISPATCH __fnCOPYGLOBALDATA;
    PFN_DISPATCH __fnEMPTY1;
    PFN_DISPATCH __fnNCDESTROY;
    PFN_DISPATCH __fnDWORDOPTINLPMSG;
    PFN_DISPATCH __fnINOUTDRAG;
    PFN_DISPATCH __fnGETTEXTLENGTHS1;
    PFN_DISPATCH __fnINCNTOUTSTRING;
    PFN_DISPATCH __fnINCNTOUTSTRINGNULL;
    PFN_DISPATCH __fnINLPCOMPAREITEMSTRUCT;
    PFN_DISPATCH __fnINLPCREATESTRUCT;
    PFN_DISPATCH __fnINLPDELETEITEMSTRUCT;
    PFN_DISPATCH __fnINLPDRAWITEMSTRUCT;
    PFN_DISPATCH __fnPOPTINLPUINT1;
    PFN_DISPATCH __fnPOPTINLPUINT2;
    PFN_DISPATCH __fnINLPMDICREATESTRUCT;
    PFN_DISPATCH __fnINOUTLPMEASUREITEMSTRUCT;
    PFN_DISPATCH __fnINLPWINDOWPOS;
    PFN_DISPATCH __fnINOUTLPPOINT51;
    PFN_DISPATCH __fnINOUTLPSCROLLINFO;
    PFN_DISPATCH __fnINOUTLPRECT;
    PFN_DISPATCH __fnINOUTNCCALCSIZE;
    PFN_DISPATCH __fnINOUTLPPOINT52;
    PFN_DISPATCH __fnINPAINTCLIPBRD;
    PFN_DISPATCH __fnINSIZECLIPBRD;
    PFN_DISPATCH __fnINDESTROYCLIPBRD;
    PFN_DISPATCH __fnINSTRINGNULL1;
    PFN_DISPATCH __fnINSTRINGNULL2;
    PFN_DISPATCH __fnINDEVICECHANGE;
    PFN_DISPATCH __fnPOWERBROADCAST;
    PFN_DISPATCH __fnINLPUAHDRAWMENU1;
    PFN_DISPATCH __fnOPTOUTLPDWORDOPTOUTLPDWORD1;
    PFN_DISPATCH __fnOPTOUTLPDWORDOPTOUTLPDWORD2;
    PFN_DISPATCH __fnOUTDWORDINDWORD;
    PFN_DISPATCH __fnOUTLPRECT;
    PFN_DISPATCH __fnOUTSTRING;
    PFN_DISPATCH __fnPOPTINLPUINT3;
    PFN_DISPATCH __fnPOUTLPINT;
    PFN_DISPATCH __fnSENTDDEMSG;
    PFN_DISPATCH __fnINOUTSTYLECHANGE1;
    PFN_DISPATCH __fnHkINDWORD;
    PFN_DISPATCH __fnHkINLPCBTACTIVATESTRUCT;
    PFN_DISPATCH __fnHkINLPCBTCREATESTRUCT;
    PFN_DISPATCH __fnHkINLPDEBUGHOOKSTRUCT;
    PFN_DISPATCH __fnHkINLPMOUSEHOOKSTRUCTEX1;
    PFN_DISPATCH __fnHkINLPKBDLLHOOKSTRUCT;
    PFN_DISPATCH __fnHkINLPMSLLHOOKSTRUCT;
    PFN_DISPATCH __fnHkINLPMSG;
    PFN_DISPATCH __fnHkINLPRECT;
    PFN_DISPATCH __fnHkOPTINLPEVENTMSG;
    PFN_DISPATCH __xxxClientCallDelegateThread;
    PFN_DISPATCH __ClientCallDummyCallback1;
    PFN_DISPATCH __ClientCallDummyCallback2;
    PFN_DISPATCH __fnSHELLWINDOWMANAGEMENTCALLOUT;
    PFN_DISPATCH __fnSHELLWINDOWMANAGEMENTNOTIFY;
    PFN_DISPATCH __ClientCallDummyCallback3;
    PFN_DISPATCH __xxxClientCallDitThread;
    PFN_DISPATCH __xxxClientEnableMMCSS;
    PFN_DISPATCH __xxxClientUpdateDpi;
    PFN_DISPATCH __xxxClientExpandStringW;
    PFN_DISPATCH __ClientCopyDDEIn1;
    PFN_DISPATCH __ClientCopyDDEIn2;
    PFN_DISPATCH __ClientCopyDDEOut1;
    PFN_DISPATCH __ClientCopyDDEOut2;
    PFN_DISPATCH __ClientCopyImage;
    PFN_DISPATCH __ClientEventCallback;
    PFN_DISPATCH __ClientFindMnemChar;
    PFN_DISPATCH __ClientFreeDDEHandle;
    PFN_DISPATCH __ClientFreeLibrary;
    PFN_DISPATCH __ClientGetCharsetInfo;
    PFN_DISPATCH __ClientGetDDEFlags;
    PFN_DISPATCH __ClientGetDDEHookData;
    PFN_DISPATCH __ClientGetListboxString;
    PFN_DISPATCH __ClientGetMessageMPH;
    PFN_DISPATCH __ClientLoadImage;
    PFN_DISPATCH __ClientLoadLibrary;
    PFN_DISPATCH __ClientLoadMenu;
    PFN_DISPATCH __ClientLoadLocalT1Fonts;
    PFN_DISPATCH __ClientPSMTextOut;
    PFN_DISPATCH __ClientLpkDrawTextEx;
    PFN_DISPATCH __ClientExtTextOutW;
    PFN_DISPATCH __ClientGetTextExtentPointW;
    PFN_DISPATCH __ClientCharToWchar;
    PFN_DISPATCH __ClientAddFontResourceW;
    PFN_DISPATCH __ClientThreadSetup;
    PFN_DISPATCH __ClientDeliverUserApc;
    PFN_DISPATCH __ClientNoMemoryPopup;
    PFN_DISPATCH __ClientMonitorEnumProc;
    PFN_DISPATCH __ClientCallWinEventProc;
    PFN_DISPATCH __ClientWaitMessageExMPH;
    PFN_DISPATCH __ClientCallDummyCallback4;
    PFN_DISPATCH __ClientCallDummyCallback5;
    PFN_DISPATCH __ClientImmLoadLayout;
    PFN_DISPATCH __ClientImmProcessKey;
    PFN_DISPATCH __fnIMECONTROL;
    PFN_DISPATCH __fnINWPARAMDBCSCHAR;
    PFN_DISPATCH __fnGETTEXTLENGTHS2;
    PFN_DISPATCH __ClientCallDummyCallback6;
    PFN_DISPATCH __ClientLoadStringW;
    PFN_DISPATCH __ClientLoadOLE;
    PFN_DISPATCH __ClientRegisterDragDrop;
    PFN_DISPATCH __ClientRevokeDragDrop;
    PFN_DISPATCH __fnINOUTMENUGETOBJECT;
    PFN_DISPATCH __ClientPrinterThunk;
    PFN_DISPATCH __fnOUTLPCOMBOBOXINFO;
    PFN_DISPATCH __fnOUTLPSCROLLBARINFO;
    PFN_DISPATCH __fnINLPUAHDRAWMENU2;
    PFN_DISPATCH __fnINLPUAHDRAWMENUITEM;
    PFN_DISPATCH __fnINLPUAHDRAWMENU3;
    PFN_DISPATCH __fnINOUTLPUAHMEASUREMENUITEM;
    PFN_DISPATCH __fnINLPUAHDRAWMENU4;
    PFN_DISPATCH __fnOUTLPTITLEBARINFOEX;
    PFN_DISPATCH __fnTOUCH;
    PFN_DISPATCH __fnGESTURE;
    PFN_DISPATCH __fnPOPTINLPUINT4;
    PFN_DISPATCH __fnPOPTINLPUINT5;
    PFN_DISPATCH __xxxClientCallDefaultInputHandler;
    PFN_DISPATCH __fnEMPTY2;
    PFN_DISPATCH __ClientRimDevCallback;
    PFN_DISPATCH __xxxClientCallMinTouchHitTestingCallback;
    PFN_DISPATCH __ClientCallLocalMouseHooks;
    PFN_DISPATCH __xxxClientBroadcastThemeChange;
    PFN_DISPATCH __xxxClientCallDevCallbackSimple;
    PFN_DISPATCH __xxxClientAllocWindowClassExtraBytes;
    PFN_DISPATCH __xxxClientFreeWindowClassExtraBytes;
    PFN_DISPATCH __fnGETWINDOWDATA;
    PFN_DISPATCH __fnINOUTSTYLECHANGE2;
    PFN_DISPATCH __fnHkINLPMOUSEHOOKSTRUCTEX2;
    PFN_DISPATCH __xxxClientCallDefWindowProc;
    PFN_DISPATCH __fnSHELLSYNCDISPLAYCHANGED;
    PFN_DISPATCH __fnHkINLPCHARHOOKSTRUCT;
    PFN_DISPATCH __fnINTERCEPTEDWINDOWACTION;
    PFN_DISPATCH __xxxTooltipCallback;
    PFN_DISPATCH __xxxClientInitPSBInfo;
    PFN_DISPATCH __xxxClientDoScrollMenu;
    PFN_DISPATCH __xxxClientEndScroll;
    PFN_DISPATCH __xxxClientDrawSize;
    PFN_DISPATCH __xxxClientDrawScrollBar;
    PFN_DISPATCH __xxxClientHitTestScrollBar;
    PFN_DISPATCH __xxxClientTrackInit;
} KERNEL_CALLBACK_TABLE, *PKERNEL_CALLBACK_TABLE;


// private // PEB->ApiSetMap since THRESHOLD
typedef struct _API_SET_NAMESPACE
{
    unsigned long Version; // API_SET_SCHEMA_VERSION_V6
    unsigned long Size;
    unsigned long Flags; // API_SET_SCHEMA_FLAGS_*
    unsigned long Count;
    unsigned long EntryOffset; // to API_SET_NAMESPACE_ENTRY[Count], from this struct base
    unsigned long HashOffset; // to API_SET_HASH_ENTRY[Count], from this struct base
    unsigned long HashFactor;
} API_SET_NAMESPACE, *PAPI_SET_NAMESPACE;


typedef struct _RTL_BITMAP* PRTL_BITMAP;
typedef struct _RTL_BITMAP
{
    unsigned long SizeOfBitMap;
    unsigned long* Buffer;
} RTL_BITMAP, *PRTL_BITMAP;

/**
 * The NT_PRODUCT_TYPE enum identifies the Windows product family for the operating system.
 */
typedef enum _NT_PRODUCT_TYPE
{
    NtProductWinNt = 1,
    NtProductLanManNt,
    NtProductServer
} NT_PRODUCT_TYPE, *PNT_PRODUCT_TYPE;


/**
 * The KSYSTEM_TIME structure represents interrupt time, system time, and time zone bias.
 */
typedef struct _KSYSTEM_TIME
{
    unsigned long LowPart;
    long High1Time;
    long High2Time;
} KSYSTEM_TIME, *PKSYSTEM_TIME;

typedef struct _SILO_USER_SHARED_DATA *PSILO_USER_SHARED_DATA;
typedef struct _SILO_USER_SHARED_DATA
{
    unsigned long                   ServiceSessionId;
    unsigned long                    ActiveConsoleId;
    long long      ConsoleSessionForegroundProcessId;
    NT_PRODUCT_TYPE                    NtProductType;
    unsigned long                          SuiteMask;
    unsigned long                SharedUserSessionId; // since RS2
    unsigned char                  IsMultiSessionSku;
    unsigned char           IsStateSeparationEnabled;
    wchar_t                        NtSystemRoot[260];
    unsigned short          UserModeGlobalLogger[16];
    unsigned long                         TimeZoneId; // since 21H2
    long                           TimeZoneBiasStamp;
    KSYSTEM_TIME                        TimeZoneBias;
    LARGE_INTEGER         TimeZoneBiasEffectiveStart;
    LARGE_INTEGER           TimeZoneBiasEffectiveEnd;
} SILO_USER_SHARED_DATA, *PSILO_USER_SHARED_DATA;


#define MAXIMUM_LEADBYTES 12
/**
 * Stores the NLS file formats.
 *
 * \sa https://learn.microsoft.com/en-us/previous-versions/mt791523(v=vs.85)
 */
typedef struct _CPTABLEINFO
{
    unsigned short CodePage;                        // Specifies the code page number.
    unsigned short MaximumCharacterSize;            // Specifies the maximum length in bytes of a character.
    unsigned short DefaultChar;                     // Specifies the default character (MB).
    unsigned short UniDefaultChar;                  // Specifies the default character (Unicode).
    unsigned short TransDefaultChar;                // Specifies the translation of the default character (Unicode).
    unsigned short TransUniDefaultChar;             // Specifies the translation of the Unicode default character (MB).
    unsigned short DBCSCodePage;                    // Specifies non-zero for DBCS code pages.
    unsigned char LeadByte[MAXIMUM_LEADBYTES];      // Specifies the lead byte ranges.
    unsigned short * MultiByteTable;                // Specifies a pointer to a MB translation table.
    void             *WideCharTable;                // Specifies a pointer to a WC translation table.
    unsigned short * DBCSRanges;                    // Specifies a pointer to DBCS ranges.
    unsigned short * DBCSOffsets;                   // Specifies a pointer to DBCS offsets.
} CPTABLEINFO, *PCPTABLEINFO;



/**
 * Stores the NLS file formats.
 *
 * \sa https://learn.microsoft.com/en-us/previous-versions/mt791531(v=vs.85)
 */
typedef struct _NLSTABLEINFO
{
    CPTABLEINFO        OemTableInfo;               // Specifies OEM table.
    CPTABLEINFO       AnsiTableInfo;               // Specifies an ANSI table.
    unsigned short * UpperCaseTable;               // Specifies an 844 format uppercase table.
    unsigned short * LowerCaseTable;               // Specifies an 844 format lowercase table.
} NLSTABLEINFO, *PNLSTABLEINFO;

typedef struct _GDI_HANDLE_ENTRY GDI_HANDLE_ENTRY, *PGDI_HANDLE_ENTRY;
typedef struct _GDI_HANDLE_ENTRY
{
    union
    {
        void* Object;
        void* NextFree;
    } DUMMYUNIONNAME;
    union
    {
        unsigned long Value;
        struct
        {
            unsigned short ProcessId;
            unsigned short Lock : 1;
            unsigned short Count : 15;
        } DUMMYSTRUCTNAME;
    } Owner;
    unsigned short Unique;
    unsigned char Type;
    unsigned char Flags;
    void* UserPointer;
} GDI_HANDLE_ENTRY, *PGDI_HANDLE_ENTRY;

typedef void (__stdcall PS_PROCESS_START_ROUTINE)(
    void* Parameter
);
typedef PS_PROCESS_START_ROUTINE* PPS_PROCESS_START_ROUTINE;


// Not the actual structure, but has the same size.
typedef struct _HEAP_ENTRY
{
    void* Data1;
    void* Data2;
} HEAP_ENTRY, *PHEAP_ENTRY;

#define HEAP_SEGMENT_SIGNATURE 0xffeeffee

// Windows 7 and above
typedef struct _HEAP_SEGMENT
{
    HEAP_ENTRY Entry;
    unsigned long SegmentSignature;
    unsigned long SegmentFlags;
    LIST_ENTRY SegmentListEntry;
    struct _HEAP *Heap;
    void* BaseAddress;
    unsigned long NumberOfPages;
    PHEAP_ENTRY FirstEntry;
    PHEAP_ENTRY LastValidEntry;
    unsigned long NumberOfUnCommittedPages;
    unsigned long NumberOfUnCommittedRanges;
    unsigned short SegmentAllocatorBackTraceIndex;
    unsigned short Reserved;
    LIST_ENTRY UCRSegmentList;
} HEAP_SEGMENT, *PHEAP_SEGMENT;

#define HEAP_SIGNATURE 0xeeffeeff
typedef struct _HEAP HEAP, *PHEAP;

typedef struct _HEAP_PSEUDO_TAG_ENTRY
{
    unsigned long long  CommitLimitBytes;
    unsigned long long  CommitLimitFailureCode;
} HEAP_PSEUDO_TAG_ENTRY, *PHEAP_PSEUDO_TAG_ENTRY;

typedef struct _RTL_RUN_ONCE { void* Ptr; } RTL_RUN_ONCE, *PRTL_RUN_ONCE;
typedef unsigned long  (__stdcall *PRTL_RUN_ONCE_INIT_FN)(PRTL_RUN_ONCE, void*, void**);

typedef struct _RTLP_HEAP_COMMIT_LIMIT_DATA
{
    unsigned long long  CommitLimitBytes;
    unsigned long long  CommitLimitFailureCode;
} RTLP_HEAP_COMMIT_LIMIT_DATA, *PRTLP_HEAP_COMMIT_LIMIT_DATA;

typedef struct _HEAP_COUNTERS
{
    unsigned long long  TotalMemoryReserved;
    unsigned long long  TotalMemoryCommitted;
    unsigned long long  TotalMemoryLargeUCR;
    unsigned long long  TotalSizeInVirtualBlocks;
    unsigned long TotalSegments;
    unsigned long TotalUCRs;
    unsigned long CommittOps;
    unsigned long DeCommitOps;
    unsigned long LockAcquires;
    unsigned long LockCollisions;
    unsigned long CommitRate;
    unsigned long DecommittRate;
    unsigned long CommitFailures;
    unsigned long InBlockCommitFailures;
    unsigned long PollIntervalCounter;
    unsigned long DecommitsSinceLastCheck;
    unsigned long HeapPollInterval;
    unsigned long AllocAndFreeOps;
    unsigned long AllocationIndicesActive;
    unsigned long InBlockDeccommits;
    unsigned long long  InBlockDeccomitSize;
    unsigned long long  HighWatermarkSize;
    unsigned long long  LastPolledSize;
} HEAP_COUNTERS, *PHEAP_COUNTERS;


typedef struct _HEAP_TUNING_PARAMETERS
{
    unsigned long CommittThresholdShift;
    unsigned long long  MaxPreCommittThreshold;
} HEAP_TUNING_PARAMETERS, *PHEAP_TUNING_PARAMETERS;


typedef struct _HEAP
{
    HEAP_SEGMENT Segment;
    HEAP_ENTRY Entry;
    unsigned long SegmentSignature;
    unsigned long SegmentFlags;
    LIST_ENTRY SegmentListEntry;
    struct _HEAP* Heap;
    void* BaseAddress;
    unsigned long NumberOfPages;
    PHEAP_ENTRY FirstEntry;
    PHEAP_ENTRY LastValidEntry;
    unsigned long NumberOfUnCommittedPages;
    unsigned long NumberOfUnCommittedRanges;
    unsigned short SegmentAllocatorBackTraceIndex;
    unsigned short Reserved;
    LIST_ENTRY UCRSegmentList;
    unsigned long Flags;
    unsigned long ForceFlags;
    unsigned long CompatibilityFlags;
    unsigned long EncodeFlagMask;
    HEAP_ENTRY Encoding;
    unsigned long Interceptor;
    unsigned long VirtualMemoryThreshold;
    unsigned long Signature;
    unsigned long long SegmentReserve;
    unsigned long long  SegmentCommit;
    unsigned long long  DeCommitFreeBlockThreshold;
    unsigned long long  DeCommitTotalFreeThreshold;
    unsigned long long  TotalFreeSize;
    unsigned long long  MaximumAllocationSize;
    unsigned short ProcessHeapsListIndex;
    unsigned short HeaderValidateLength;
    void* HeaderValidateCopy;
    unsigned short NextAvailableTagIndex;
    unsigned short MaximumTagIndex;
    struct _HEAP_TAG_ENTRY* TagEntries;
    LIST_ENTRY UCRList;
    unsigned long long  AlignRound;
    unsigned long long  AlignMask;
    LIST_ENTRY VirtualAllocdBlocks;
    LIST_ENTRY SegmentList;
    unsigned short AllocatorBackTraceIndex;
    unsigned long NonDedicatedListLength;
    void* BlocksIndex;
    void* UCRIndex;
    PHEAP_PSEUDO_TAG_ENTRY PseudoTagEntries;
    LIST_ENTRY FreeLists;
    struct _HEAP_LOCK* LockVariable;
    long (*CommitRoutine)(void);
    RTL_RUN_ONCE StackTraceInitVar;
    RTLP_HEAP_COMMIT_LIMIT_DATA CommitLimitData;
    void* UserContext;
    uintptr_t Spare;
    void* FrontEndHeap;
    unsigned short FrontHeapLockCount;
    unsigned char FrontEndHeapType;
    unsigned char RequestedFrontEndHeapType;
    unsigned short* FrontEndHeapUsageData;
    unsigned short FrontEndHeapMaximumIndex;
    unsigned char FrontEndHeapStatusBitmap[129];
    unsigned char ReadOnly : 1;
    unsigned char InternalFlags;
    HEAP_COUNTERS Counters;
    HEAP_TUNING_PARAMETERS TuningParameters;
} HEAP;


typedef struct _RTL_CRITICAL_SECTION
{
    struct _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
    long        LockCount;
    long        RecursionCount;
    void*       OwningThread;
    void*       LockSemaphore;
    uintptr_t   SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef void (__stdcall PS_POST_PROCESS_INIT_ROUTINE)(void);

typedef PS_POST_PROCESS_INIT_ROUTINE* PPS_POST_PROCESS_INIT_ROUTINE;

typedef struct _SHIM_PROCESS_CONTEXT SHIM_PROCESS_CONTEXT, *PSHIM_PROCESS_CONTEXT;


typedef struct _GUID {
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;

typedef struct tagSDBQUERYRESULT
{
    unsigned long Exes[16];
    unsigned long ExeFlags[16];
    unsigned long Layers[8];
    unsigned long LayerFlags;
    unsigned long AppHelp;
    unsigned long ExeCount;
    unsigned long LayerCount;
    GUID ID;
    unsigned long ExtraFlags;
    unsigned long CustomSDBMap;
    GUID DB[16];
} SDBQUERYRESULT, *PSDBQUERYRESULT;



typedef struct tagSWITCH_CONTEXT_ATTRIBUTE
{
    uintptr_t ContextUpdateCounter;
    int AllowContextUpdate;
    int EnableTrace;
    void* EtwHandle;
} SWITCH_CONTEXT_ATTRIBUTE, *PSWITCH_CONTEXT_ATTRIBUTE;

#ifdef _WIN64
static_assert(sizeof(SWITCH_CONTEXT_ATTRIBUTE) == 0x18, "SWITCH_CONTEXT_ATTRIBUTE size mismatch");
#else
static_assert(sizeof(SWITCH_CONTEXT_ATTRIBUTE) == 0x10, "SWITCH_CONTEXT_ATTRIBUTE size mismatch");
#endif

typedef struct tagSWITCH_CONTEXT_DATA
{
    unsigned long long  OsMaxVersionTested;
    unsigned long TargetPlatform;
    unsigned long long  ContextMinimum;
    GUID Platform;
    GUID MinPlatform;
    unsigned long ContextSource;
    unsigned long ElementCount;
    GUID Elements[48];
} SWITCH_CONTEXT_DATA, * PSWITCH_CONTEXT_DATA;

static_assert(sizeof(SWITCH_CONTEXT_DATA) == 0x340, "SWITCH_CONTEXT_DATA size mismatch");

typedef struct tagSWITCH_CONTEXT
{
    SWITCH_CONTEXT_ATTRIBUTE Attribute;
    SWITCH_CONTEXT_DATA Data;
} SWITCH_CONTEXT, *PSWITCH_CONTEXT;


typedef struct _SDB_CSTRUCT_COBALT_PROCFLAG
{
    unsigned long long  AffinityMask;
    unsigned long CPUIDEcxOverride;
    unsigned long CPUIDEdxOverride;
    unsigned short ProcessorGroup;
    unsigned short FastSelfModThreshold;
    unsigned short Reserved1;
    unsigned char Reserved2;
    unsigned char BackgroundWork : 5;
    unsigned char CPUIDBrand : 4;
    unsigned char Reserved3 : 4;
    unsigned char RdtscScaling : 3;
    unsigned char Reserved4 : 2;
    unsigned char UnalignedAtomicApproach : 2;
    unsigned char Win11Atomics : 2;
    unsigned char RunOnSingleCore : 1;
    unsigned char X64CPUID : 1;
    unsigned char PatchUnaligned : 1;
    unsigned char InterpreterOrJitter : 1;
    unsigned char ForceSegmentHeap : 1;
    unsigned char Reserved5 : 1;
    unsigned char Reserved6 : 1;
    union
    {
        unsigned long long Group1AsUINT64;
        struct _SDB_CSTRUCT_COBALT_PROCFLAG* Specified;
    };
} SDB_CSTRUCT_COBALT_PROCFLAG, *PSDB_CSTRUCT_COBALT_PROCFLAG;


typedef struct _APPCOMPAT_EXE_DATA APPCOMPAT_EXE_DATA, *PAPPCOMPAT_EXE_DATA;
typedef struct _APPCOMPAT_EXE_DATA
{
    uintptr_t Reserved[65];
    unsigned long  Size;
    unsigned long Magic;
    int LoadShimEngine;
    unsigned short ExeType;
    SDBQUERYRESULT SdbQueryResult;
    unsigned long long  DbgLogChannels[128];
    SWITCH_CONTEXT SwitchContext;
    unsigned long ParentProcessId;
    wchar_t ParentImageName[260];
    wchar_t ParentCompatLayers[256];
    wchar_t ActiveCompatLayers[256];
    unsigned long ImageFileSize;
    unsigned long ImageCheckSum;
    int LatestOs;
    int PackageId;
    int SwitchBackManifest;
    int UacManifest;
    int LegacyInstaller;
    unsigned long RunLevel;
    unsigned long long  WinRTFlags;
    void* HookCOM;
    void* ComponentOnDemandEvent;
    void* Quirks;
    unsigned long QuirksSize;
    SDB_CSTRUCT_COBALT_PROCFLAG CobaltProcFlags;
    unsigned long FullMatchDbSizeCb;
    unsigned long FullMatchDbOffset;
} APPCOMPAT_EXE_DATA;


typedef struct _WER_HEAP_MAIN_HEADER
{
    wchar_t Signature[16];
    LIST_ENTRY Links;
    void* Mutex;
    void* FreeHeap;
    unsigned long FreeCount;
} WER_HEAP_MAIN_HEADER, *PWER_HEAP_MAIN_HEADER;

typedef struct _WER_RECOVERY_INFO
{
    unsigned Length;
    void* Callback;
    void* Parameter;
    void* Started;
    void* Finished;
    void* InProgress;
    long LastError;
    int Successful;
    unsigned long PingInterval;
    unsigned long Flags;
} WER_RECOVERY_INFO, *PWER_RECOVERY_INFO;

#ifndef DOS_MAX_COMPONENT_LENGTH
#define DOS_MAX_COMPONENT_LENGTH 255
#endif

#ifndef DOS_MAX_PATH_LENGTH
#define DOS_MAX_PATH_LENGTH (DOS_MAX_COMPONENT_LENGTH + 5)
#endif

typedef struct _WER_FILE
{
    unsigned short Flags;
    wchar_t Path[DOS_MAX_PATH_LENGTH];
} WER_FILE, *PWER_FILE;

typedef struct _WER_MEMORY
{
    void* Address;
    unsigned long Size;
} WER_MEMORY, *PWER_MEMORY;

typedef struct _WER_GATHER
{
    void* Next;
    unsigned short Flags;
    union
    {
        WER_FILE File;
        WER_MEMORY Memory;
    } v;
} WER_GATHER, *PWER_GATHER;

typedef struct _WER_METADATA
{
    void* Next;
    wchar_t Key[64];
    wchar_t Value[128];
} WER_METADATA, *PWER_METADATA;

typedef struct _WER_RUNTIME_DLL
{
    void* Next;
    unsigned long Length;
    void* Context;
    wchar_t CallbackDllPath[DOS_MAX_PATH_LENGTH];
} WER_RUNTIME_DLL, *PWER_RUNTIME_DLL;


typedef struct _WER_DUMP_COLLECTION
{
    void* Next;
    unsigned long ProcessId;
    unsigned long ThreadId;
} WER_DUMP_COLLECTION, *PWER_DUMP_COLLECTION;

#ifndef RESTART_MAX_CMD_LINE
#define RESTART_MAX_CMD_LINE 1024
#endif
typedef struct _WER_PEB_HEADER_BLOCK
{
    long Length;
    wchar_t Signature[16];
    wchar_t AppDataRelativePath[64];
    wchar_t RestartCommandLine[RESTART_MAX_CMD_LINE];
    WER_RECOVERY_INFO RecoveryInfo;
    PWER_GATHER Gather;
    PWER_METADATA MetaData;
    PWER_RUNTIME_DLL RuntimeDll;
    PWER_DUMP_COLLECTION DumpCollection;
    long GatherCount;
    long MetaDataCount;
    long DumpCount;
    long Flags;
    WER_HEAP_MAIN_HEADER MainHeader;
    void* Reserved;
} WER_PEB_HEADER_BLOCK, *PWER_PEB_HEADER_BLOCK;


typedef struct _LDR_PATCH_TABLE
{
    uintptr_t Version;
    void* LdrLoadDll;
    void* LdrGetProcedureAddress;
} LDR_PATCH_TABLE, *PLDR_PATCH_TABLE;

typedef struct _WER_REGISTRATION_DATA
{
    void* Context;
    void* CallbackContext;
    void* Callback;
    wchar_t Name[DOS_MAX_PATH_LENGTH];
} WER_REGISTRATION_DATA, *PWER_REGISTRATION_DATA;

typedef struct _RTL_SRWLOCK { void* Ptr; } RTL_SRWLOCK,*PRTL_SRWLOCK;
typedef struct _RTL_CONDITION_VARIABLE { void* Ptr; } RTL_CONDITION_VARIABLE,*PRTL_CONDITION_VARIABLE;

#define ANYSIZE_ARRAY 1
// PEB->TelemetryCoverageHeader
typedef struct _TELEMETRY_COVERAGE_HEADER
{
    unsigned char MajorVersion;
    unsigned char MinorVersion;
    struct
    {
        unsigned short TracingEnabled : 1;
        unsigned short Reserved1 : 15;
    };
    unsigned long HashTableEntries;
    unsigned long HashIndexMask;
    unsigned long TableUpdateVersion;
    unsigned long TableSizeInBytes;
    unsigned long LastResetTick;
    unsigned long ResetRound;
    unsigned long Reserved2;
    unsigned long RecordedCount;
    unsigned long Reserved3[4];
    unsigned long HashTable[ANYSIZE_ARRAY];
} TELEMETRY_COVERAGE_HEADER, *PTELEMETRY_COVERAGE_HEADER;

typedef struct _LEAP_SECOND_DATA *PLEAP_SECOND_DATA;

// PEB->AppCompatFlags
#define KACF_OLDGETSHORTPATHNAME                      0x00000001
#define KACF_VERSIONLIE_NOT_USED                      0x00000002
#define KACF_GETTEMPPATH_NOT_USED                     0x00000004
#define KACF_GETDISKFREESPACE                         0x00000008
#define KACF_APPCOMPATFLAG_BIT4                       0x00000010
#define KACF_FTMFROMCURRENTAPT                        0x00000020
#define KACF_DISALLOWORBINDINGCHANGES                 0x00000040
#define KACF_OLE32VALIDATEPTRS                        0x00000080
#define KACF_DISABLECICERO                            0x00000100
#define KACF_OLE32ENABLEASYNCDOCFILE                  0x00000200
#define KACF_OLE32ENABLELEGACYEXCEPTIONHANDLING       0x00000400
#define KACF_RPCDISABLENDRCLIENTHARDENING             0x00000800
#define KACF_RPCDISABLENDRMAYBENULL_SIZEIS            0x00001000
#define KACF_DISABLEALLDDEHACK_NOT_USED               0x00002000
#define KACF_RPCDISABLENDR61_RANGE                    0x00004000
#define KACF_RPC32ENABLELEGACYEXCEPTIONHANDLING       0x00008000
#define KACF_OLE32DOCFILEUSELEGACYNTFSFLAGS           0x00010000
#define KACF_RPCDISABLENDRCONSTIIDCHECK               0x00020000
#define KACF_USERDISABLEFORWARDERPATCH                0x00040000
#define KACF_APPCOMPATFLAG_BIT19                      0x00080000
#define KACF_OLE32DISABLENEW_WMPAINT_DISPATCH         0x00100000
#define KACF_ADDRESTRICTEDSIDINCOINITIALIZESECURITY   0x00200000
#define KACF_ALLOCDEBUGINFOFORCRITSECTIONS            0x00400000
#define KACF_OLEAUT32ENABLEUNSAFELOADTYPELIBRELATIVE  0x00800000
#define KACF_ALLOWMAXIMIZEDWINDOWGAMMA                0x01000000
#define KACF_DONOTADDTOCACHE                          0x80000000
#define KACF_DISABLEPOSIXDELETEFILE                   0x100000000 // rev KernelBase!InternalDeleteFileW
#define KACF_ENABLE_PROCESS_SYSTEMDPIAWARENESS        0x20000000000000 // rev // Enable Per-Process System DPI Awareness and Opt-in to Per-Process System DPI mode.
#define KACF_DISABLE_PROCESS_SYSTEMDPIAWARENESS       0x40000000000000 // rev // Disable Per-Process System DPI Awareness and force legacy DPI behavior.
#define KACF_ENABLE_GDI_DPI_SCALING                   0x800000000000000
#define KACF_FORCE_DISABLE_GDI_SCALING                0x4000000000000000

// PEB->CrossProcessFlags
#define PEB_FLAG_PROCESS_IN_JOB                       0x00000001 // Process is part of a job
#define PEB_FLAG_PROCESS_INITIALIZING                 0x00000002 // Process is initializing
#define PEB_FLAG_PROCESS_USING_VEH                    0x00000004 // Process is using VEH
#define PEB_FLAG_PROCESS_USING_VCH                    0x00000008 // Process is using VCH
#define PEB_FLAG_PROCESS_USING_FTH                    0x00000010 // Process is using FTH
#define PEB_FLAG_PROCESS_PREVIOUSLY_THROTTLED         0x00000020 // Process was previously throttled
#define PEB_FLAG_PROCESS_CURRENTLY_THROTTLED          0x00000040 // Process is currently throttled
#define PEB_FLAG_PROCESS_IMAGES_HOT_PATCHED           0x00000080 // Process images are hot patched (RS5+)

//  Process Environment Block (PEB) structure.
typedef struct _PEB
{
    //
    // The process was cloned with an inherited address space.
    //
    unsigned char InheritedAddressSpace;

    //
    // The process has image file execution options (IFEO).
    //
    unsigned char ReadImageFileExecOptions;

    //
    // The process has a debugger attached.
    // If the current process is being attached by a debugger (such as x64dbg, IDA pro, VS), 
    // this value will be set to 1 by the system. In normal circumstances, it is 0.
    //
    unsigned char BeingDebugged;

    union
    {
        unsigned char BitField;
        struct
        {
            unsigned char ImageUsesLargePages : 1;            // The process uses large image regions (4 MB).
            unsigned char IsProtectedProcess : 1;             // The process is a protected process.
            unsigned char IsImageDynamicallyRelocated : 1;    // The process image base address was relocated.
            unsigned char SkipPatchingUser32Forwarders : 1;   // The process skipped forwarders for User32.dll functions. 1 for 64-bit, 0 for 32-bit.
            unsigned char IsPackagedProcess : 1;              // The process is a packaged store process (APPX/MSIX).
            unsigned char IsAppContainerProcess : 1;          // The process has an AppContainer token.
            unsigned char IsProtectedProcessLight : 1;        // The process is a protected process (light).
            unsigned char IsLongPathAwareProcess : 1;         // The process is long path aware.
        };
    };

    //
    // Handle to a mutex for synchronization.
    //
    void* Mutant;

    //
    // Pointer to the base address of the process image.
    //
    void* ImageBaseAddress;

    //
    // Pointer to the process loader data.
    //
    PPEB_LDR_DATA Ldr;

    //
    // Pointer to the process parameters.
    //
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;

    //
    // Reserved.
    //
    void* SubSystemData;

    //
    // Pointer to the process default heap.
    //
    PHEAP ProcessHeap;

    //
    // Pointer to a critical section used to synchronize access to the PEB.
    //
    PRTL_CRITICAL_SECTION FastPebLock;

    //
    // Pointer to a singly linked list used by ATL.
    //
    PSLIST_HEADER AtlThunkSListPtr;

    //
    // Handle to the Image File Execution Options key.
    //
    void* IFEOKey;

    //
    // Cross process flags.
    //
    union
    {
        unsigned long CrossProcessFlags;
        struct
        {
            unsigned long ProcessInJob : 1;                 // The process is part of a job.
            unsigned long ProcessInitializing : 1;          // The process is initializing.
            unsigned long ProcessUsingVEH : 1;              // The process is using VEH.
            unsigned long ProcessUsingVCH : 1;              // The process is using VCH.
            unsigned long ProcessUsingFTH : 1;              // The process is using FTH.
            unsigned long ProcessPreviouslyThrottled : 1;   // The process was previously throttled.
            unsigned long ProcessCurrentlyThrottled : 1;    // The process is currently throttled.
            unsigned long ProcessImagesHotPatched : 1;      // The process images are hot patched. // RS5
            unsigned long ReservedBits0 : 24;
        };
    };

    //
    // User32 KERNEL_CALLBACK_TABLE (ntuser.h)
    //
    union
    {
        PKERNEL_CALLBACK_TABLE KernelCallbackTable;
        void* UserSharedInfoPtr;
    };

    //
    // Reserved.
    //
    unsigned long SystemReserved;

    //
    // Pointer to the Active Template Library (ATL) singly linked list (32-bit)
    //
    unsigned long AtlThunkSListPtr32;

    //
    // Pointer to the API Set Schema.
    //
    PAPI_SET_NAMESPACE ApiSetMap;

    //
    // Counter for TLS expansion.
    //
    unsigned long TlsExpansionCounter;

    //
    // Pointer to the TLS bitmap.
    //
    PRTL_BITMAP TlsBitmap;

    //
    // Bits for the TLS bitmap.
    //
    unsigned long TlsBitmapBits[2];

    //
    // Reserved for CSRSS.
    //
    void* ReadOnlySharedMemoryBase;

    //
    // Pointer to the USER_SHARED_DATA for the current SILO.
    //
    PSILO_USER_SHARED_DATA SharedData;

    //
    // Reserved for CSRSS.
    //
    void** ReadOnlyStaticServerData;

    //
    // Pointer to the ANSI code page data.
    //
    PCPTABLEINFO AnsiCodePageData;

    //
    // Pointer to the OEM code page data.
    //
    PCPTABLEINFO OemCodePageData;

    //
    // Pointer to the Unicode case table data.
    //
    PNLSTABLEINFO UnicodeCaseTableData;

    //
    // The total number of system processors.
    //
    unsigned long NumberOfProcessors;

    //
    // Global flags for the system.
    //
    union
    {
        unsigned long NtGlobalFlag;
        struct
        {
            unsigned long StopOnException : 1;          // FLG_STOP_ON_EXCEPTION
            unsigned long ShowLoaderSnaps : 1;          // FLG_SHOW_LDR_SNAPS
            unsigned long DebugInitialCommand : 1;      // FLG_DEBUG_INITIAL_COMMAND
            unsigned long StopOnHungGUI : 1;            // FLG_STOP_ON_HUNG_GUI
            unsigned long HeapEnableTailCheck : 1;      // FLG_HEAP_ENABLE_TAIL_CHECK
            unsigned long HeapEnableFreeCheck : 1;      // FLG_HEAP_ENABLE_FREE_CHECK
            unsigned long HeapValidateParameters : 1;   // FLG_HEAP_VALIDATE_PARAMETERS
            unsigned long HeapValidateAll : 1;          // FLG_HEAP_VALIDATE_ALL
            unsigned long ApplicationVerifier : 1;      // FLG_APPLICATION_VERIFIER
            unsigned long MonitorSilentProcessExit : 1; // FLG_MONITOR_SILENT_PROCESS_EXIT
            unsigned long PoolEnableTagging : 1;        // FLG_POOL_ENABLE_TAGGING
            unsigned long HeapEnableTagging : 1;        // FLG_HEAP_ENABLE_TAGGING
            unsigned long UserStackTraceDb : 1;         // FLG_USER_STACK_TRACE_DB
            unsigned long KernelStackTraceDb : 1;       // FLG_KERNEL_STACK_TRACE_DB
            unsigned long MaintainObjectTypeList : 1;   // FLG_MAINTAIN_OBJECT_TYPELIST
            unsigned long HeapEnableTagByDll : 1;       // FLG_HEAP_ENABLE_TAG_BY_DLL
            unsigned long DisableStackExtension : 1;    // FLG_DISABLE_STACK_EXTENSION
            unsigned long EnableCsrDebug : 1;           // FLG_ENABLE_CSRDEBUG
            unsigned long EnableKDebugSymbolLoad : 1;   // FLG_ENABLE_KDEBUG_SYMBOL_LOAD
            unsigned long DisablePageKernelStacks : 1;  // FLG_DISABLE_PAGE_KERNEL_STACKS
            unsigned long EnableSystemCritBreaks : 1;   // FLG_ENABLE_SYSTEM_CRIT_BREAKS
            unsigned long HeapDisableCoalescing : 1;    // FLG_HEAP_DISABLE_COALESCING
            unsigned long EnableCloseExceptions : 1;    // FLG_ENABLE_CLOSE_EXCEPTIONS
            unsigned long EnableExceptionLogging : 1;   // FLG_ENABLE_EXCEPTION_LOGGING
            unsigned long EnableHandleTypeTagging : 1;  // FLG_ENABLE_HANDLE_TYPE_TAGGING
            unsigned long HeapPageAllocs : 1;           // FLG_HEAP_PAGE_ALLOCS
            unsigned long DebugInitialCommandEx : 1;    // FLG_DEBUG_INITIAL_COMMAND_EX
            unsigned long DisableDbgPrint : 1;          // FLG_DISABLE_DBGPRINT
            unsigned long CritSecEventCreation : 1;     // FLG_CRITSEC_EVENT_CREATION
            unsigned long LdrTopDown : 1;               // FLG_LDR_TOP_DOWN
            unsigned long EnableHandleExceptions : 1;   // FLG_ENABLE_HANDLE_EXCEPTIONS
            unsigned long DisableProtDlls : 1;          // FLG_DISABLE_PROTDLLS
        } NtGlobalFlags;
    };

    //
    // Timeout for critical sections.
    //
    LARGE_INTEGER CriticalSectionTimeout;

    //
    // Reserved size for heap segments.
    //
    uintptr_t HeapSegmentReserve;

    //
    // Committed size for heap segments.
    //
    uintptr_t HeapSegmentCommit;

    //
    // Threshold for decommitting total free heap.
    //
    uintptr_t HeapDeCommitTotalFreeThreshold;

    //
    // Threshold for decommitting free heap blocks.
    //
    uintptr_t HeapDeCommitFreeBlockThreshold;

    //
    // Number of process heaps.
    //
    unsigned long NumberOfHeaps;

    //
    // Maximum number of process heaps.
    //
    unsigned long MaximumNumberOfHeaps;

    //
    // Pointer to an array of process heaps. ProcessHeaps is initialized
    // to point to the first free byte after the PEB and MaximumNumberOfHeaps
    // is computed from the page size used to hold the PEB, less the fixed
    // size of this data structure.
    //
    void* ProcessHeaps;

    //
    // Pointer to the system GDI shared handle table.
    //
    PGDI_HANDLE_ENTRY GdiSharedHandleTable;

    //
    // Pointer to the process starter helper.
    //
    PPS_PROCESS_START_ROUTINE ProcessStarterHelper;

    //
    // The maximum number of GDI function calls during batch operations (GdiSetBatchLimit)
    //
    unsigned long GdiDCAttributeList;

    //
    // Pointer to the loader lock critical section.
    //
    PRTL_CRITICAL_SECTION LoaderLock;

    //
    // Major version of the operating system.
    //
    unsigned long OSMajorVersion;

    //
    // Minor version of the operating system.
    //
    unsigned long OSMinorVersion;

    //
    // Build number of the operating system.
    //
    unsigned short OSBuildNumber;

    //
    // CSD version of the operating system.
    //
    unsigned short OSCSDVersion;

    //
    // Platform ID of the operating system.
    //
    unsigned long OSPlatformId;

    //
    // Subsystem version of the current process image (PE Headers).
    //
    unsigned long ImageSubsystem;

    //
    // Major version of the current process image subsystem (PE Headers).
    //
    unsigned long ImageSubsystemMajorVersion;

    //
    // Minor version of the current process image subsystem (PE Headers).
    //
    unsigned long ImageSubsystemMinorVersion;

    //
    // Affinity mask for the current process.
    //
    unsigned long long ActiveProcessAffinityMask;

    //
    // Temporary buffer for GDI handles accumulated in the current batch.
    //
    GDI_HANDLE_BUFFER GdiHandleBuffer;

    //
    // Pointer to the post-process initialization routine available for use by the application.
    //
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;

    //
    // Pointer to the TLS expansion bitmap.
    //
    PRTL_BITMAP TlsExpansionBitmap;

    //
    // Bits for the TLS expansion bitmap. TLS_EXPANSION_SLOTS
    //
    unsigned long TlsExpansionBitmapBits[32];

    //
    // Session ID of the current process.
    //
    unsigned long SessionId;

    //
    // Application compatibility flags. KACF_*
    //
    ULARGE_INTEGER AppCompatFlags;

    //
    // Application compatibility flags. KACF_*
    //
    ULARGE_INTEGER AppCompatFlagsUser;

    //
    // Pointer to the Application SwitchBack Compatibility Engine.
    //
    PSHIM_PROCESS_CONTEXT pShimData;

    //
    // Pointer to the Application Compatibility Engine.
    //
    PAPPCOMPAT_EXE_DATA AppCompatInfo;

    //
    // CSD version string of the operating system.
    //
    UNICODE_STRING CSDVersion;

    //
    // Pointer to the process activation context.
    //
    PACTIVATION_CONTEXT_DATA ActivationContextData;

    //
    // Pointer to the process assembly storage map.
    //
    PASSEMBLY_STORAGE_MAP ProcessAssemblyStorageMap;

    //
    // Pointer to the system default activation context.
    //
    PACTIVATION_CONTEXT_DATA SystemDefaultActivationContextData;

    //
    // Pointer to the system assembly storage map.
    //
    PASSEMBLY_STORAGE_MAP SystemAssemblyStorageMap;

    //
    // Minimum stack commit size.
    //
    uintptr_t MinimumStackCommit;

    //
    // since 19H1 (previously FlsCallback to FlsHighIndex)
    //
    void* SparePointers[2];

    //
    // Pointer to the patch loader data.
    //
    PLDR_PATCH_TABLE PatchLoaderData;

    //
    // Pointer to the CHPE V2 process information. CHPEV2_PROCESS_INFO
    //
    void* ChpeV2ProcessInfo;

    //
    // Packaged process feature state.
    //
    unsigned long AppModelFeatureState;

    //
    // SpareUlongs
    //
    unsigned long SpareUlongs[2];

    //
    // Active code page.
    //
    unsigned short ActiveCodePage;

    //
    // OEM code page.
    //
    unsigned short OemCodePage;

    //
    // Code page case mapping.
    //
    unsigned short UseCaseMapping;

    //
    // Unused NLS field.
    //
    unsigned short UnusedNlsField;

    //
    // Pointer to the application WER registration data.
    //
    PWER_PEB_HEADER_BLOCK WerRegistrationData;

    //
    // Pointer to the application WER assert pointer.
    //
    PWER_REGISTRATION_DATA WerShipAssertPtr;

    //
    // Pointer to the EC bitmap on ARM64. (Windows 11 and above)
    //
    union
    {
        void* pContextData; // Pointer to the switchback compatibility engine (Windows 7 and below)
        void* EcCodeBitMap; // Pointer to the EC bitmap on ARM64 (Windows 11 and above) // since WIN11
    };

    //
    // Reserved.
    //
    void* ImageHeaderHash;

    //
    // ETW tracing flags.
    //
    union
    {
        unsigned long TracingFlags;
        struct
        {
            unsigned long HeapTracingEnabled : 1;       // ETW heap tracing enabled.
            unsigned long CritSecTracingEnabled : 1;    // ETW lock tracing enabled.
            unsigned long LibLoaderTracingEnabled : 1;  // ETW loader tracing enabled.
            unsigned long SpareTracingBits : 29;
        };
    };

    //
    // Reserved for CSRSS.
    //
    unsigned long long CsrServerReadOnlySharedMemoryBase;

    //
    // Thread pool worker list lock.
    //
    PRTL_SRWLOCK TppWorkerpListLock;

    //
    // Thread pool worker list.
    //
    LIST_ENTRY TppWorkerpList;

    //
    // Wait on address hash table. (RtlWaitOnAddress)
    //
    void* WaitOnAddressHashTable[128];

    //
    // Pointer to the telemetry coverage header. // since RS3
    //
    PTELEMETRY_COVERAGE_HEADER TelemetryCoverageHeader;

    //
    // Cloud file flags. (ProjFs and Cloud Files) // since RS4
    //
    unsigned long CloudFileFlags;

    //
    // Cloud file diagnostic flags.
    //
    unsigned long CloudFileDiagFlags;

    //
    // Placeholder compatibility mode. (ProjFs and Cloud Files)
    //
    char PlaceholderCompatibilityMode;

    //
    // Reserved for placeholder compatibility mode.
    //
    char PlaceholderCompatibilityModeReserved[7];

    //
    // Pointer to leap second data. // since RS5
    //
    PLEAP_SECOND_DATA LeapSecondData;

    //
    // Leap second flags.
    //
    union
    {
        unsigned long LeapSecondFlags;
        struct
        {
            unsigned long SixtySecondEnabled : 1; // Leap seconds enabled.
            unsigned long Reserved : 31;
        };
    };

    //
    // Global flags for the process.
    //
    unsigned long NtGlobalFlag2;

    //
    // Extended feature disable mask (AVX). // since WIN11
    //
    unsigned long long ExtendedFeatureDisableMask;
} PEB, *PPEB;

#ifdef _WIN64
static_assert(FIELD_OFFSET(PEB, SessionId) == 0x2C0, "FIELD_OFFSET(PEB, SessionId) is incorrect");
static_assert(sizeof(PEB) == 0x7d0, "Size of PEB is incorrect"); // WIN11
#else
static_assert(FIELD_OFFSET(PEB, SessionId) == 0x1D4, "FIELD_OFFSET(PEB, SessionId) is incorrect");
static_assert(sizeof(PEB) == 0x488, "Size of PEB is incorrect"); // WIN11
#endif


#endif // !__NT_PEB_TEB_HPP__