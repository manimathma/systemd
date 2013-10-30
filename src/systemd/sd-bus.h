/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

#ifndef foosdbushfoo
#define foosdbushfoo

/***
  This file is part of systemd.

  Copyright 2013 Lennart Poettering

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include <inttypes.h>
#include <sys/types.h>

#include "sd-id128.h"
#include "sd-event.h"
#include "sd-memfd.h"
#include "_sd-common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Types */

typedef struct sd_bus sd_bus;
typedef struct sd_bus_message sd_bus_message;

typedef struct {
        const char *name;
        const char *message;
        int need_free;
} sd_bus_error;

/* Callbacks */

typedef int (*sd_bus_message_handler_t)(sd_bus *bus, sd_bus_message *m, void *userdata);
typedef int (*sd_bus_property_get_t) (sd_bus *bus, const char *path, const char *interface, const char *property, sd_bus_message *reply, sd_bus_error *error, void *userdata);
typedef int (*sd_bus_property_set_t) (sd_bus *bus, const char *path, const char *interface, const char *property, sd_bus_message *value, sd_bus_error *error, void *userdata);
typedef int (*sd_bus_object_find_t) (sd_bus *bus, const char *path, const char *interface, void **found, void *userdata);
typedef int (*sd_bus_node_enumerator_t) (sd_bus *bus, const char *path, char ***nodes, void *userdata);

#include "sd-bus-protocol.h"
#include "sd-bus-vtable.h"

/* Connections */

int sd_bus_open_user(sd_bus **ret);
int sd_bus_open_system(sd_bus **ret);
int sd_bus_open_system_remote(const char *host, sd_bus **ret);

int sd_bus_new(sd_bus **ret);
int sd_bus_set_address(sd_bus *bus, const char *address);
int sd_bus_set_fd(sd_bus *bus, int input_fd, int output_fd);
int sd_bus_set_exec(sd_bus *bus, const char *path, char *const argv[]);
int sd_bus_set_bus_client(sd_bus *bus, int b);
int sd_bus_set_server(sd_bus *bus, int b, sd_id128_t server_id);
int sd_bus_set_anonymous(sd_bus *bus, int b);
int sd_bus_negotiate_fds(sd_bus *bus, int b);
int sd_bus_negotiate_attach_comm(sd_bus *bus, int b);
int sd_bus_negotiate_attach_exe(sd_bus *bus, int b);
int sd_bus_negotiate_attach_cmdline(sd_bus *bus, int b);
int sd_bus_negotiate_attach_cgroup(sd_bus *bus, int b);
int sd_bus_negotiate_attach_caps(sd_bus *bus, int b);
int sd_bus_negotiate_attach_selinux_context(sd_bus *bus, int b);
int sd_bus_negotiate_attach_audit(sd_bus *bus, int b);
int sd_bus_start(sd_bus *ret);

void sd_bus_close(sd_bus *bus);

sd_bus *sd_bus_ref(sd_bus *bus);
sd_bus *sd_bus_unref(sd_bus *bus);

int sd_bus_is_open(sd_bus *bus);
int sd_bus_can_send(sd_bus *bus, char type);
int sd_bus_get_server_id(sd_bus *bus, sd_id128_t *peer);

int sd_bus_send(sd_bus *bus, sd_bus_message *m, uint64_t *serial);
int sd_bus_send_with_reply(sd_bus *bus, sd_bus_message *m, sd_bus_message_handler_t callback, void *userdata, uint64_t usec, uint64_t *serial);
int sd_bus_send_with_reply_cancel(sd_bus *bus, uint64_t serial);
int sd_bus_send_with_reply_and_block(sd_bus *bus, sd_bus_message *m, uint64_t usec, sd_bus_error *error, sd_bus_message **r);

int sd_bus_get_fd(sd_bus *bus);
int sd_bus_get_events(sd_bus *bus);
int sd_bus_get_timeout(sd_bus *bus, uint64_t *timeout_usec);
int sd_bus_process(sd_bus *bus, sd_bus_message **r);
int sd_bus_wait(sd_bus *bus, uint64_t timeout_usec);
int sd_bus_flush(sd_bus *bus);

int sd_bus_attach_event(sd_bus *bus, sd_event *e, int priority);
int sd_bus_detach_event(sd_bus *bus);

int sd_bus_add_filter(sd_bus *bus, sd_bus_message_handler_t callback, void *userdata);
int sd_bus_remove_filter(sd_bus *bus, sd_bus_message_handler_t callback, void *userdata);

int sd_bus_add_match(sd_bus *bus, const char *match, sd_bus_message_handler_t callback, void *userdata);
int sd_bus_remove_match(sd_bus *bus, const char *match, sd_bus_message_handler_t callback, void *userdata);

int sd_bus_add_object(sd_bus *bus, const char *path, sd_bus_message_handler_t callback, void *userdata);
int sd_bus_remove_object(sd_bus *bus, const char *path, sd_bus_message_handler_t callback, void *userdata);

int sd_bus_add_fallback(sd_bus *bus, const char *prefix, sd_bus_message_handler_t callback, void *userdata);
int sd_bus_remove_fallback(sd_bus *bus, const char *prefix, sd_bus_message_handler_t callback, void *userdata);

int sd_bus_add_object_vtable(sd_bus *bus, const char *path, const char *interface, const sd_bus_vtable *vtable, void *userdata);
int sd_bus_remove_object_vtable(sd_bus *bus, const char *path, const char *interface);

int sd_bus_add_fallback_vtable(sd_bus *bus, const char *path, const char *interface, const sd_bus_vtable *vtable, sd_bus_object_find_t find, void *userdata);
int sd_bus_remove_fallback_vtable(sd_bus *bus, const char *path, const char *interface);

int sd_bus_add_node_enumerator(sd_bus *bus, const char *path, sd_bus_node_enumerator_t callback, void *userdata);
int sd_bus_remove_node_enumerator(sd_bus *bus, const char *path, sd_bus_node_enumerator_t callback, void *userdata);

int sd_bus_add_object_manager(sd_bus *bus, const char *path);
int sd_bus_remove_object_manager(sd_bus *bus, const char *path);

/* Message object */
int sd_bus_message_new_signal(sd_bus *bus, const char *path, const char *interface, const char *member, sd_bus_message **m);
int sd_bus_message_new_method_call(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_message **m);
int sd_bus_message_new_method_return(sd_bus *bus, sd_bus_message *call, sd_bus_message **m);
int sd_bus_message_new_method_error(sd_bus *bus, sd_bus_message *call, const sd_bus_error *e, sd_bus_message **m);
int sd_bus_message_new_method_errorf(sd_bus *bus, sd_bus_message *call, sd_bus_message **m, const char *name, const char *format, ...) _sd_printf_(5, 0);
int sd_bus_message_new_method_errno(sd_bus *bus, sd_bus_message *call, int error, const sd_bus_error *e, sd_bus_message **m);
int sd_bus_message_new_method_errnof(sd_bus *bus, sd_bus_message *call, sd_bus_message **m, int error, const char *format, ...) _sd_printf_(5, 0);

sd_bus_message* sd_bus_message_ref(sd_bus_message *m);
sd_bus_message* sd_bus_message_unref(sd_bus_message *m);

int sd_bus_message_get_type(sd_bus_message *m, uint8_t *type);
int sd_bus_message_get_serial(sd_bus_message *m, uint64_t *serial);
int sd_bus_message_get_reply_serial(sd_bus_message *m, uint64_t *serial);
int sd_bus_message_get_no_reply(sd_bus_message *m);

const char *sd_bus_message_get_signature(sd_bus_message *m, int complete);
const char *sd_bus_message_get_path(sd_bus_message *m);
const char *sd_bus_message_get_interface(sd_bus_message *m);
const char *sd_bus_message_get_member(sd_bus_message *m);
const char *sd_bus_message_get_destination(sd_bus_message *m);
const char *sd_bus_message_get_sender(sd_bus_message *m);
const sd_bus_error *sd_bus_message_get_error(sd_bus_message *m);
int sd_bus_message_get_errno(sd_bus_message *m);

int sd_bus_message_get_monotonic_timestamp(sd_bus_message *m, uint64_t *usec);
int sd_bus_message_get_realtime_timestamp(sd_bus_message *m, uint64_t *usec);
int sd_bus_message_get_uid(sd_bus_message *m, uid_t *uid);
int sd_bus_message_get_gid(sd_bus_message *m, gid_t *gid);
int sd_bus_message_get_pid(sd_bus_message *m, pid_t *pid);
int sd_bus_message_get_tid(sd_bus_message *m, pid_t *tid);
int sd_bus_message_get_pid_starttime(sd_bus_message *m, uint64_t *usec);
int sd_bus_message_get_selinux_context(sd_bus_message *m, const char **r);
int sd_bus_message_get_comm(sd_bus_message *m, const char **r);
int sd_bus_message_get_tid_comm(sd_bus_message *m, const char **r);
int sd_bus_message_get_exe(sd_bus_message *m, const char **r);
int sd_bus_message_get_cgroup(sd_bus_message *m, const char **r);
int sd_bus_message_get_cmdline(sd_bus_message *m, char ***cmdline);
int sd_bus_message_get_unit(sd_bus_message *m, const char **unit);
int sd_bus_message_get_user_unit(sd_bus_message *m, const char **unit);
int sd_bus_message_get_session(sd_bus_message *m, const char **session);
int sd_bus_message_get_owner_uid(sd_bus_message *m, uid_t *uid);
int sd_bus_message_get_audit_sessionid(sd_bus_message *m, uint32_t *sessionid);
int sd_bus_message_get_audit_loginuid(sd_bus_message *m, uid_t *loginuid);
int sd_bus_message_has_effective_cap(sd_bus_message *m, int capability);

int sd_bus_message_is_signal(sd_bus_message *m, const char *interface, const char *member);
int sd_bus_message_is_method_call(sd_bus_message *m, const char *interface, const char *member);
int sd_bus_message_is_method_error(sd_bus_message *m, const char *name);

int sd_bus_message_set_no_reply(sd_bus_message *m, int b);
int sd_bus_message_set_destination(sd_bus_message *m, const char *destination);

int sd_bus_message_append(sd_bus_message *m, const char *types, ...);
int sd_bus_message_append_basic(sd_bus_message *m, char type, const void *p);
int sd_bus_message_append_array(sd_bus_message *m, char type, const void *ptr, size_t size);
int sd_bus_message_append_array_space(sd_bus_message *m, char type, size_t size, void **ptr);
int sd_bus_message_append_array_memfd(sd_bus_message *m, char type, sd_memfd *memfd);
int sd_bus_message_append_string_space(sd_bus_message *m, size_t size, char **s);
int sd_bus_message_append_string_memfd(sd_bus_message *m, sd_memfd* memfd);
int sd_bus_message_append_strv(sd_bus_message *m, char **l);
int sd_bus_message_open_container(sd_bus_message *m, char type, const char *contents);
int sd_bus_message_close_container(sd_bus_message *m);
int sd_bus_message_copy(sd_bus_message *m, sd_bus_message *source, int all);

int sd_bus_message_read(sd_bus_message *m, const char *types, ...);
int sd_bus_message_read_basic(sd_bus_message *m, char type, void *p);
int sd_bus_message_read_array(sd_bus_message *m, char type, const void **ptr, size_t *size);
int sd_bus_message_enter_container(sd_bus_message *m, char type, const char *contents);
int sd_bus_message_exit_container(sd_bus_message *m);
int sd_bus_message_peek_type(sd_bus_message *m, char *type, const char **contents);
int sd_bus_message_verify_type(sd_bus_message *m, char type, const char *contents);
int sd_bus_message_rewind(sd_bus_message *m, int complete);

/* Convenience calls */

int sd_bus_call_method(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *error, sd_bus_message **reply, const char *types, ...);
int sd_bus_get_property(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *error, sd_bus_message **reply, const char *type);
int sd_bus_set_property(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *error, const char *type, ...);
int sd_bus_reply_method_return(sd_bus *bus, sd_bus_message *call, const char *types, ...);
int sd_bus_reply_method_error(sd_bus *bus, sd_bus_message *call, const sd_bus_error *e);
int sd_bus_reply_method_errorf(sd_bus *bus, sd_bus_message *call, const char *name, const char *format, ...) _sd_printf_(4, 0);
int sd_bus_reply_method_errno(sd_bus *bus, sd_bus_message *call, int error, const sd_bus_error *e);
int sd_bus_reply_method_errnof(sd_bus *bus, sd_bus_message *call, int error, const char *format, ...) _sd_printf_(4, 0);

int sd_bus_emit_signal(sd_bus *bus, const char *path, const char *interface, const char *member, const char *types, ...);

int sd_bus_emit_properties_changed_strv(sd_bus *bus, const char *path, const char *interface, char **names);
int sd_bus_emit_properties_changed(sd_bus *bus, const char *path, const char *interface, const char *name, ...) _sd_sentinel_;

int sd_bus_emit_interfaces_added_strv(sd_bus *bus, const char *path, char **interfaces);
int sd_bus_emit_interfaces_added(sd_bus *bus, const char *path, const char *interface, ...) _sd_sentinel_;
int sd_bus_emit_interfaces_removed_strv(sd_bus *bus, const char *path, char **interfaces);
int sd_bus_emit_interfaces_removed(sd_bus *bus, const char *path, const char *interface, ...) _sd_sentinel_;

/* Bus management */

int sd_bus_get_unique_name(sd_bus *bus, const char **unique);
int sd_bus_request_name(sd_bus *bus, const char *name, int flags);
int sd_bus_release_name(sd_bus *bus, const char *name);
int sd_bus_list_names(sd_bus *bus, char ***l);
int sd_bus_get_owner(sd_bus *bus, const char *name, char **owner);
int sd_bus_get_owner_uid(sd_bus *bus, const char *name, uid_t *uid);
int sd_bus_get_owner_pid(sd_bus *bus, const char *name, pid_t *pid);
int sd_bus_get_owner_machine_id(sd_bus *bus, const char *name, sd_id128_t *machine);

/* Error structures */

#define SD_BUS_ERROR_MAKE(name, message) ((sd_bus_error) {(name), (message), 0})
#define SD_BUS_ERROR_NULL SD_BUS_ERROR_MAKE(NULL, NULL)

void sd_bus_error_free(sd_bus_error *e);
int sd_bus_error_set(sd_bus_error *e, const char *name, const char *message);
int sd_bus_error_setf(sd_bus_error *e, const char *name, const char *format, ...)  _sd_printf_(3, 0);
int sd_bus_error_set_const(sd_bus_error *e, const char *name, const char *message);
int sd_bus_error_set_errno(sd_bus_error *e, int error);
int sd_bus_error_set_errnof(sd_bus_error *e, int error, const char *format, ...) _sd_printf_(3, 0);
int sd_bus_error_get_errno(const sd_bus_error *e);
int sd_bus_error_copy(sd_bus_error *dest, const sd_bus_error *e);
int sd_bus_error_is_set(const sd_bus_error *e);
int sd_bus_error_has_name(const sd_bus_error *e, const char *name);

#define SD_BUS_MESSAGE_APPEND_ID128(x) 16,                              \
                (x).bytes[0],  (x).bytes[1],  (x).bytes[2],  (x).bytes[3], \
                (x).bytes[4],  (x).bytes[5],  (x).bytes[6],  (x).bytes[7], \
                (x).bytes[8],  (x).bytes[9],  (x).bytes[10], (x).bytes[11], \
                (x).bytes[12], (x).bytes[13], (x).bytes[14], (x).bytes[15]

#define SD_BUS_MESSAGE_READ_ID128(x) 16,                                \
                &(x).bytes[0],  &(x).bytes[1],  &(x).bytes[2],  &(x).bytes[3], \
                &(x).bytes[4],  &(x).bytes[5],  &(x).bytes[6],  &(x).bytes[7], \
                &(x).bytes[8],  &(x).bytes[9],  &(x).bytes[10], &(x).bytes[11], \
                &(x).bytes[12], &(x).bytes[13], &(x).bytes[14], &(x).bytes[15]

#ifdef __cplusplus
}
#endif

#endif
