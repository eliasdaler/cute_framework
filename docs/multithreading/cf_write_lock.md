[](../header.md ':include')

# cf_write_lock

Category: [multithreading](/api_reference?id=multithreading)  
GitHub: [cute_multithreading.h](https://github.com/RandyGaul/cute_framework/blob/master/include/cute_multithreading.h)  
---

Locks for writing.

```cpp
void cf_write_lock(CF_ReadWriteLock* rw);
```

Parameters | Description
--- | ---
rw | The read/write lock.

## Remarks

When locked for writing, only one writer can be present with no readers. The writer will sleep/wait for all other
readers and writers to unlock before acquiring exclusive access to the lock.

## Related Pages

[CF_ReadWriteLock](/multithreading/cf_readwritelock.md)  
[cf_make_rw_lock](/multithreading/cf_make_rw_lock.md)  
[cf_destroy_rw_lock](/multithreading/cf_destroy_rw_lock.md)  
[cf_read_lock](/multithreading/cf_read_lock.md)  
[cf_read_unlock](/multithreading/cf_read_unlock.md)  
[cf_write_unlock](/multithreading/cf_write_unlock.md)  
