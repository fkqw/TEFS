/*

 */
#ifndef TFS_COMMON_FILE_QUEUE_H_
#define TFS_COMMON_FILE_QUEUE_H_

#include "internal.h"
#include "tbsys.h"

namespace tfs
{
  namespace common
  {
    static const int32_t FILE_QUEUE_MAX_FILE_SIZE = 1024 * 1024 * 16; //16M
    static const int32_t FILE_QUEUE_MAX_THREAD_SIZE = 0x0f;
    struct unsettle
    {
      int32_t seqno_;
      int32_t offset_;
    };
    struct QueueItem
    {
      unsettle pos_;
      int32_t length_;
      char data_[0];
    };
    struct QueueInformationHeader
    {
      int32_t read_seqno_;
      int32_t read_offset_;
      int32_t write_seqno_;
      int32_t write_filesize_;
      int32_t queue_size_;
      int32_t exit_status_;
      int32_t reserve_[2];
      unsettle pos_[FILE_QUEUE_MAX_THREAD_SIZE];
    };

    class FileQueue
    {
      friend class FileQueueThread;
    public:

      FileQueue(const std::string& dir_path, const std::string& queue_name, const int32_t max_file_size =
          FILE_QUEUE_MAX_FILE_SIZE);

      virtual ~FileQueue();

      int load_queue_head();

      int initialize();

      int push(const void* const data, const int64_t len);

      QueueItem *pop(const int32_t index = 0);

      //QueueItem *pop(int32_t& offset, const int32_t index);

      int clear();

      bool empty() const;

      void finish(const int index = 0);

      inline void set_delete_file_flag(const bool flags = true)
      {
        delete_file_flag_ = flags;
      }

      inline QueueInformationHeader* get_queue_information_header()
      {
        return &queue_information_header_;
      }
      inline const QueueInformationHeader* get_queue_information_header() const
      {
        return &queue_information_header_;
      }

      inline int update_queue_information_header(const QueueInformationHeader* head)
      {
        memcpy(&queue_information_header_, head, sizeof(queue_information_header_));
        int iret = write_header();
        if (iret != TFS_SUCCESS)
        {
          TBSYS_LOG(TFERROR, "write queue information header error, iret: %d", iret);
          return iret;
        }
        iret = open_read_file();
        if (iret != TFS_SUCCESS)
        {
          TBSYS_LOG(TFERROR, "open file failed, iret: %d", iret);
        }
        return iret;
      }
    private:
      DISALLOW_COPY_AND_ASSIGN( FileQueue);
      int open_write_file();
      int open_read_file();
      int delete_read_file();
      int write_header();
      int recover_record();

    private:
      int32_t read_fd_;
      int32_t write_fd_;
      int32_t information_fd_;
      QueueInformationHeader queue_information_header_;
      int32_t max_file_size_;
      bool delete_file_flag_;
      std::string queue_path_;
    };
  }//end namespace common
}// end namesapce tfs
#endif  // TFS_COMMON_FILE_QUEUE_H_