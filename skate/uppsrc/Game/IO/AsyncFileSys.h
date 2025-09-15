/*****************************************************************************
**																			**
**			              Neversoft Entertainment	                        **
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		Sys Library												**
**																			**
**	Module:			File													**
**																			**
**	Created:		10/11/02	-	grj										**
**																			**
**	File name:		core/sys/p_AsyncFilesys.h									**
**																			**
*****************************************************************************/

#ifndef	__SYS_FILE_P_ASYNC_FILESYS_H
#define	__SYS_FILE_P_ASYNC_FILESYS_H

/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

// Internal async file system types (memory kinds, function ids)
// Note: kept local to this header to avoid pulling the global aggregator.
#include "PRE.h"
#include "AsyncTypes.h"

#include <Game/Components/Task.h>
#include <Game/Components/Module2.h>
#include <Game/MainLoop.h>


/*****************************************************************************
**								   Defines									**
*****************************************************************************/

namespace File
{



/*****************************************************************************
**							     Type Defines								**
*****************************************************************************/

/*****************************************************************************
**							 Private Declarations							**
*****************************************************************************/

/*****************************************************************************
**							  Private Prototypes							**
*****************************************************************************/

/*****************************************************************************
**							  Public Declarations							**
*****************************************************************************/

/*****************************************************************************
**							   Public Prototypes							**
*****************************************************************************/

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

// Minimal task wrapper implementation used by async poller
namespace Tsk {
template <class T>
class Task : public ::Tsk::BaseTask {
public:
    using Fn = void (*)(const Tsk::Task<T>&);
    Task(Fn fn, Mdl::Module& mdl, Node::Priority p = Node::vNORMAL_PRIORITY) : m_fn(fn), mp_owner(static_cast<T*>(&mdl)), ::Tsk::BaseTask(p) {}
    void vCall(void) const override { if (m_fn) m_fn(*this); }
    void* GetCode(void) const override { return reinterpret_cast<void*>(m_fn); }
    void Remove() {}
    const T& Owner() const { return *mp_owner; }
    T& Owner() { return *mp_owner; }
private:
    Fn  m_fn{nullptr};
    T*  mp_owner{nullptr};
};
} // namespace Tsk

class CAsyncFileHandle;

// Asynchronous I/O manager (static-only service)
class CAsyncFileLoader {
public:
    // Configuration limits
    enum { MAX_FILE_HANDLES = 32, MAX_PENDING_CALLBACKS = 128 };

    // Lifecycle
    static void           sInit();
    static void           sCleanup();

    // Capabilities
    static bool           sAsyncSupported();
    static bool           sAsyncInUse();
    static bool           sExist(const char* filename);

    // Open/close
    static CAsyncFileHandle* sOpen(const char* filename, bool blocking, int priority=-1);
    static bool              sClose(CAsyncFileHandle* p_file_handle);

    // Event/dispatch
    static void           sWaitForIOEvent(bool all_io_events);
    static void           s_update();
    static void           s_execute_callback_list();

    // Global busy/completion state
    static volatile int   s_manager_busy_count;
    static volatile bool  s_new_io_completion;

//private:
    struct SCallback {
        CAsyncFileHandle*     mp_file_handle;
        EAsyncFunctionType    m_function;
        int                   m_result;
    };

    static CAsyncFileHandle* s_get_file_handle();
    static bool              s_free_file_handle(CAsyncFileHandle* p_file_handle);
    static void              s_add_callback(CAsyncFileHandle* p_file, EAsyncFunctionType function, int result);

    // Platform hooks
    static void           s_plat_init();
    static void           s_plat_cleanup();
    static bool           s_plat_async_supported();
    static bool           s_plat_exist(const char* filename);
    static void           s_plat_update();
    static void           s_plat_swap_callback_list();

//private:
    static CAsyncFileHandle* s_file_handles[MAX_FILE_HANDLES];
    static int               s_free_handle_index;

    static volatile SCallback s_callback_list[2][MAX_PENDING_CALLBACKS];
    static volatile int       s_num_callbacks[2];
    static int                s_cur_callback_list_index;
};

// Asynchronous file handle
class CAsyncFileHandle {
public:
    // Callback signature: file, function, result, user_arg0, user_arg1
    typedef void (*AsyncCallback)(CAsyncFileHandle*, EAsyncFunctionType, int, unsigned int, unsigned int);

    // Limits and defaults (buffer/priority are configured at runtime)
    static const uint32   MAX_FILE_SIZE;        // defined in .cpp
    static const size_t   DEFAULT_BUFFER_SIZE = 64 * 1024;
    static const int      DEFAULT_ASYNC_PRIORITY = 0;

    CAsyncFileHandle();
    ~CAsyncFileHandle();

    void                  init();

    // State
    volatile bool         IsBusy();
    int                   WaitForIO();

    // Parameters
    void                  SetPriority(int priority);
    void                  SetStream(bool stream);
    void                  SetDestination(EAsyncMemoryType destination);
    void                  SetBufferSize(size_t buffer_size);
    void                  SetBlocking(bool block);
    void                  SetCallback(AsyncCallback p_callback, unsigned int arg0, unsigned int arg1);

    // File ops
    size_t                GetFileSize();
    size_t                Load(void* p_buffer);
    size_t                Read(void* p_buffer, size_t size, size_t count);
    size_t                Write(void* p_buffer, size_t size, size_t count);
    int                   Seek(long offset, int origin);
    bool                  open(const char* filename, bool blocking, int priority);
    bool                  close();

    // Internal I/O completion
    void                  io_callback(EAsyncFunctionType function, int result, uint32 data);
    void                  post_io_callback();

    // Busy count helpers
    int                   get_busy_count() const { return m_busy_count; }
    int                   inc_busy_count()       { ++m_busy_count; ++CAsyncFileLoader::s_manager_busy_count; return m_busy_count; }
    int                   dec_busy_count()       { --m_busy_count; --CAsyncFileLoader::s_manager_busy_count; return m_busy_count; }

    // Exposed for callback thunk
    AsyncCallback         mp_callback;
    unsigned int          m_callback_arg0;
    unsigned int          m_callback_arg1;

private:
    // Platform layer
    void                  plat_init();
    bool                  plat_open(const char* filename);
    bool                  plat_close();
    volatile bool         plat_is_done();
    volatile bool         plat_is_busy();
    bool                  plat_is_eof() const;
    void                  plat_set_priority(int priority);
    void                  plat_set_stream(bool stream);
    void                  plat_set_destination(EAsyncMemoryType destination);
    void                  plat_set_buffer_size(size_t buffer_size);
    void                  plat_set_blocking(bool block);
    size_t                plat_load(void* p_buffer);
    size_t                plat_read(void* p_buffer, size_t size, size_t count);
    size_t                plat_write(void* p_buffer, size_t size, size_t count);
    char*                 plat_get_s(char* p_buffer, int maxlen);
    int                   plat_seek(long offset, int origin);

private:
    // Parameters
    EAsyncFunctionType    m_current_function;
    EAsyncMemoryType      m_mem_destination;
    bool                  m_stream;
    bool                  m_blocking;
    size_t                m_buffer_size;
    int                   m_priority;

    // Status
    volatile int          m_busy_count;
    int                   m_last_result;

    // File info
    long                  m_file_size;
    long                  m_position;
    PreFile::FileHandle*  mp_pre_file;
};

// Poller module to integrate updates into the main loop
class CAsyncFilePoll : public Mdl::Module {
public:
    CAsyncFilePoll();
    ~CAsyncFilePoll();

    static void           s_logic_code(const Tsk::Task< CAsyncFilePoll >& task);

    // Module lifecycle hooks (called by module framework)
    void                  v_start_cb();
    void                  v_stop_cb();
	
	DeclareSingletonClass(CAsyncFilePoll)
	
private:
    Tsk::Task< CAsyncFilePoll >* mp_logic_task;
};

} // namespace File

#endif  // __SYS_FILE_P_ASYNC_FILESYS_H

