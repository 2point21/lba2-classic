//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  DLLLOAD.C                                                             лл
//лл                                                                        лл
//лл  32-bit DLL driver loader                                              лл
//лл                                                                        лл
//лл  V1.00 of 16-Aug-92: Initial version for Watcom C                      лл
//лл  V1.01 of  1-May-93: Zortech C++ v3.1 compatibility added              лл
//лл  V1.02 of 16-Nov-93: Metaware High C/C++ v3.1 compatibility added      лл
//лл                                                                        лл
//лл  Project: 386FX Sound & Light(TM)                                      лл
//лл   Author: John Lemberger                                               лл
//лл                                                                        лл
//лл  C source compatible with Watcom C386 v9.0 or later                    лл
//лл                           Zortech C++ v3.1 or later                    лл
//лл                           MetaWare High C/C++ v3.1 or later            лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  Copyright (C) 1991-1993 Miles Design, Inc.                            лл
//лл                                                                        лл
//лл  Miles Design, Inc.                                                    лл
//лл  6702 Cat Creek Trail                                                  лл
//лл  Austin, TX 78731                                                      лл
//лл  (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990              лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "adeline.h"
#include "lib_sys.h"

//
// Entry table types
//

#define UNUSED_ENTRY       0
#define BIT16_ENTRY        1
#define CALL_GATE          2
#define BIT32_ENTRY        3
#define FORWARDER_ENTRY    4
#define PARAMETER_TYPING   0x80

//
// Fixup record source
//

#define BYTE_FIXUP          0x00
#define BIT16_SELECTOR      0x02
#define BIT16_POINTER       0x03
#define BIT16_OFFSET        0x05
#define BIT32_POINTER       0x06
#define BIT32_OFFSET        0x07
#define BIT32_RELATIVE      0x08
#define FIXUP_ALIAS         0x10
#define SRC_LIST_FLAG       0x20

//
// Fixup record flags
//

#define INT_REF             0x00
#define IMP_REF_BY_ORD      0x01
#define IMP_REF_BY_NAME     0x02
#define INT_REF_ENTRY_TBL   0x03
#define ADDITIVE_FIXUP      0x04
#define BIT32_TARGET        0x10
#define BIT32_ADDITIVE      0x20
#define BIT16_OBJECT        0x40
#define BIT8_ORDINAL        0x80

typedef struct
{
   ULONG lxbw;
   ULONG format_level;
   ULONG cpu_os_type;
   ULONG module_version;
   ULONG module_flags;
   ULONG module_num_of_pages;
   ULONG eip_object_num;
   ULONG eip;
   ULONG esp_object_num;
   ULONG esp;
   ULONG page_size;
   ULONG page_offset_shift;
   ULONG fixup_section_size;
   ULONG fixup_section_chksum;
   ULONG loader_section_size;
   ULONG loader_section_chksum;
   ULONG object_table_off;
   ULONG num_objects_in_module;
   ULONG object_page_table_off;
   ULONG object_iter_pages_off;
   ULONG resource_table_off;
   ULONG num_resource_table_entries;
   ULONG resident_name_table_off;
   ULONG entry_table_off;
   ULONG module_directives_off;
   ULONG num_module_directives;
   ULONG fixup_page_table_off;
   ULONG fixup_record_table_off;
   ULONG import_module_table_off;
   ULONG num_import_mod_entries;
   ULONG import_proc_table_off;
   ULONG per_page_chksum_off;
   ULONG data_pages_off;
   ULONG num_preload_pages;
   ULONG nonres_name_table_off;
   ULONG nonres_name_table_len;
   ULONG nonres_name_table_chksum;
   ULONG auto_ds_object_num;
   ULONG debug_info_off;
   ULONG debug_info_len;
   ULONG numinstance_preload;
   ULONG numinstance_demand;
   ULONG heapsize;
}
LX_header_struct;

typedef struct
{
   ULONG virtual_size;
   ULONG reloc_base_addr;
   ULONG object_flags;
   ULONG page_table_index;
   ULONG num_page_table_entries;
   ULONG reserved_space;
}
object_table_struct;

typedef struct
{
   ULONG page_data_offset;
   UWORD  data_size;
   UWORD  flags;
}
object_page_table_struct;

typedef struct
{
   UWORD  type_id;
   UWORD  name_id;
   ULONG resource_size;
   UWORD  object;
   ULONG offset;
}
resource_table_struct;

static LONG disk_err = 0;

#undef min
#define min(a,b) ((a) < (b) ? (a) : (b))

/**********************************************************/
static void *cdecl DLL_read(ULONG src, ULONG srcoff, ULONG flags,
   void *dest, ULONG length)
{
   if (flags & DLLSRC_MEM)
      {
      memcpy(dest,(BYTE *) src+srcoff,length); 
      return (void *) (srcoff+length);
      }
   else
      {
      lseek(src,(ULONG) srcoff,SEEK_SET);   // get LX header offset
      read(src,dest,length);

      return (void *) (srcoff+length);
      }
}

/**********************************************************/
ULONG cdecl DLL_size(void *source, ULONG flags)
{
   BYTE cword[]="  \0";
   void *src_ptr;
   LX_header_struct LX_hdr;
   object_table_struct object_table;
   ULONG lx;
   ULONG i;
   ULONG LX_offset;
   ULONG module_size=0;

   if (flags & DLLSRC_MEM)
      lx=(ULONG) source;
   else
      {
      lx=open((BYTE *) source,O_RDONLY | O_BINARY);

      if (lx==-1)                           // error opening file?
         return 0;
      }

   //
   // Get LX header offset
   //

   src_ptr=DLL_read(lx, 0x03c, flags, &LX_offset, 4);

   //
   // Check for valid LX marker
   //

   src_ptr=DLL_read(lx,LX_offset, flags, cword, 2);
   if (strcmp(cword,"LX"))
      {
      //
      // Error: Invalid LX file
      //

      close(lx);
      return 0;
      }

   //
   // Read LX header (Tables not included)
   //

   src_ptr=DLL_read(lx,LX_offset, flags, &LX_hdr, sizeof(LX_hdr));

   //
   // Read object table; calculate memory needed
   //

   src_ptr=(void *)(LX_offset+LX_hdr.object_table_off);

   for(i=0;i<LX_hdr.num_objects_in_module;i++)
      {
      src_ptr=DLL_read(lx,(ULONG)src_ptr, flags, &object_table, sizeof(object_table));
      module_size+=object_table.virtual_size;
      }

   if (!(flags & DLLSRC_MEM))
      {
      close(lx);
      }

   // Add slack for paragraph alignment of each DATA Object
   //
   module_size += 15 * LX_hdr.num_objects_in_module;

   return module_size;
}

/**********************************************************/
void *cdecl DLL_load(void *source, ULONG flags, void *dll)
{
   LX_header_struct LX_hdr;
   void *src_ptr;
   void *object_table_pos,*object_page_table_pos,*fixup_page_table_pos;
   object_table_struct object_table;
   object_page_table_struct object_page_table;
   BYTE cword[]="  \0";
   UBYTE *dll_ptr;
   UBYTE *object_ptr[10],*page_ptr[100];
   UBYTE src,fix_flags,f_object;
   UBYTE *trg_val;
   UWORD  srcoff,trgoff;
   ULONG read_size, dll_size;
   ULONG i,j,lx,page_count=0;
   ULONG LX_offset;
   ULONG *fix_src_ptr;
   ULONG page_offset,next_page_offset;

   if (flags & DLLSRC_MEM)
      lx=(ULONG) source;
   else
      {
      lx=open((BYTE *) source,O_RDONLY | O_BINARY);
      if (lx==-1)                           // error opening file?
	 return NULL;
      }

   //
   // Allocate memory if not done by caller
   //

   dll_size=DLL_size(source,flags);

   if (flags & DLLMEM_ALLOC)
      {
      dll=DosMalloc(dll_size, NULL);

      if (dll==NULL)
	 {
	 if (!(flags & DLLSRC_MEM))
	    close(lx);

	 return NULL;
	 }
      }

   memset(dll,0,dll_size);

   dll_ptr=dll;

   //
   // Get LX header offset
   //

   src_ptr=DLL_read(lx, 0x03c, flags, &LX_offset, 4);

   //
   // Check for valid LX marker
   //

   src_ptr=DLL_read(lx,LX_offset, flags, cword, 2);
   if (strcmp(cword,"LX"))
      {
      //
      // Error: Invalid LX file
      //

      if (!(flags & DLLSRC_MEM))
	 close(lx);

      return NULL;
      }

   //
   // Read LX header (Tables not included)
   //

   src_ptr=DLL_read(lx,LX_offset, flags, &LX_hdr, sizeof(LX_hdr));

   //
   // Read object table
   //

   src_ptr=(void *)(LX_offset+LX_hdr.object_table_off);

   for(i=0;i<LX_hdr.num_objects_in_module;i++)
      {
      src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
         &object_table, sizeof(object_table));

      //
      // Read in object page entries
      //

      object_table_pos=src_ptr;

      src_ptr=(void *)(LX_offset+LX_hdr.object_page_table_off);
      for (j=0;j<object_table.num_page_table_entries;j++)
         {
         src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
            &object_page_table,sizeof(object_page_table));

         //
         // Read page data into DLL buffer
         //

         if (j==0)   // Do if this is a new object
            {
            //
            // If this is the data object, paragraph-align it,
            // unless this is the first object in the file, then
	    // the user is responsible for allocating a paragraph aligned
	    // memory block.  JL & JM 12/30/93
            //
            // Modification made because UltraSound drivers did not load
            // properly

            if (object_table.object_flags & 0x02 &&
                object_table.object_flags & 0x01)
               {
               if ((ULONG) dll_ptr & 0x0f && i!=0)
                  dll_ptr += (0x10-((ULONG) dll & 0x0f));
               }
            object_ptr[i+1]=dll_ptr;            // Save object start position
            }

         page_ptr[page_count++] = dll_ptr;      // Save the page start position
         object_page_table_pos = src_ptr;       // Save place in table

         read_size = min(object_table.virtual_size-(dll_ptr-object_ptr[i+1]),
            object_page_table.data_size);

         src_ptr=(void *)(
            (object_page_table.page_data_offset << LX_hdr.page_offset_shift)
            +LX_hdr.data_pages_off);
         src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,dll_ptr,read_size);

         dll_ptr += read_size;

         src_ptr=(void *)object_page_table_pos; // Restore place in table
         }

      src_ptr=(void *)object_table_pos;         // Restore place in table
      }

   //
   // Read fixup page table and perform fixups
   //

   src_ptr = (void *) (LX_offset+LX_hdr.fixup_page_table_off);
   src_ptr = DLL_read(lx,(ULONG) src_ptr,flags,
         &page_offset,sizeof(page_offset));

   for(i=0;i<LX_hdr.module_num_of_pages;i++)
      {
      src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
         &next_page_offset,sizeof(next_page_offset));

      if (page_offset!=next_page_offset)
         {

         //
         // Read object page entries
         //

         fixup_page_table_pos=src_ptr;
   
         src_ptr=(void *)(page_offset+LX_offset+LX_hdr.fixup_record_table_off);
         do
            {
            src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
               &src,1);
            src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
               &fix_flags,1);

            if (!(src&BIT32_OFFSET))
               {
               //
               // Error: Non-32-bit offset fixup in table
               //

               if (!(flags & DLLSRC_MEM))
                  close(lx);

               return NULL;
               }

            if ((fix_flags & ADDITIVE_FIXUP) && !(fix_flags & BIT32_ADDITIVE))
               {
               //
               // Error: Non-32-bit additive in table
               //

               if (!(flags & DLLSRC_MEM))
                  close(lx);

               return NULL;
               }

            if (src & SRC_LIST_FLAG)
               {
               //
	       // Error: Fixup list type not supported
               //

               if (!(flags & DLLSRC_MEM))
                  close(lx);

               return NULL;
               }
            else
               {
               src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
                  &srcoff,2);
               src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
                  &f_object,1);
               src_ptr=DLL_read(lx,(ULONG)src_ptr,flags,
                  &trgoff,2);
   
               //
               // Perform fixup (if positive offset)
               //
   
               if ( (ULONG) srcoff <= LX_hdr.page_size )
                  {
                  fix_src_ptr=(ULONG *)(page_ptr[i]+srcoff);
                  trg_val=object_ptr[f_object]+trgoff;

                  *fix_src_ptr=(ULONG)trg_val;
		  }
	       }

	    } while(src_ptr < (void *)(next_page_offset+LX_offset+LX_hdr.fixup_record_table_off));

	 src_ptr=(void *)fixup_page_table_pos;
	 }

      page_offset=next_page_offset;

      }
      if (!(flags & DLLSRC_MEM))
	 close(lx);

   return dll;
}

/****************************************************************************/
LONG cdecl FILE_error(void)
{
   return disk_err;
}

/****************************************************************************/
LONG cdecl FILE_size(BYTE *filename)
{
   LONG handle;
   LONG len;

   disk_err = 0;

   handle = open(filename,O_RDONLY | O_BINARY);
   if (handle==-1)
      {
      disk_err = FILE_NOT_FOUND;
      return -1L;
      }

   len = filelength(handle);
   if (len==-1L) disk_err = CANT_READ_FILE;

   close(handle);
   return len;
}

/****************************************************************************/
void * cdecl FILE_read(BYTE *filename, void *dest)
{
   LONG i,handle;
   LONG len;
   BYTE *buf, *mem;

   disk_err = 0;

   len = FILE_size(filename);
   if (len==-1L)
      {
      disk_err = FILE_NOT_FOUND;
      return NULL;
      }

   buf = mem = (dest==NULL)? Malloc(len) : dest;

   if (buf==NULL)
      {
      disk_err = OUT_OF_MEMORY;
      return NULL;
      }

   handle = open(filename,O_RDONLY | O_BINARY);
   if (handle==-1)
      {
      Free(mem);
      disk_err = FILE_NOT_FOUND;
      return NULL;
      }

   i = read(handle,buf,len);

   if (i != len)
      {
      Free(mem);
      disk_err = CANT_READ_FILE;
      return NULL;
      }

   close(handle);
   return mem;
}

/****************************************************************************/
LONG cdecl FILE_write(BYTE *filename, void *buf, ULONG len)
{
   LONG i,handle;

   disk_err = 0;

   handle = open(filename,O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
      S_IREAD | S_IWRITE);
   if (handle==-1)
      {
      disk_err = CANT_WRITE_FILE;
      return 0;
      }

   i = write(handle,buf,len);
   if (i == -1)
      {
      disk_err = CANT_WRITE_FILE;
      return 0;
      }

   if (i != len)
      {
      disk_err = DISK_FULL;
      return 0;
      }

   close(handle);
   
   return 1;
}

/****************************************************************************/
LONG cdecl FILE_append(BYTE *filename, void *buf, ULONG len)
{
   LONG i,handle;

   disk_err = 0;

   handle = open(filename,O_APPEND | O_RDWR | O_BINARY);
   if (handle==-1)
      {
      disk_err = FILE_NOT_FOUND;
      return 0;
      }

   i = write(handle,buf,len);
   if (i == -1)
      {
      disk_err = CANT_WRITE_FILE;
      return 0;
      }

   if (i != len)
      {
      disk_err = DISK_FULL;
      return 0;
      }

   close(handle);

   return 1;
}

