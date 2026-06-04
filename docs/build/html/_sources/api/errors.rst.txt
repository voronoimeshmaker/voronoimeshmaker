Errors API
==========

``vmm::error::MeshException``
   Exception type carrying structured VMM error data.

``vmm::error::ErrorId``
   Open error identifier.

``vmm::error::ErrorRegistry``
   Registry of built-in error descriptors. Messages are intended to be
   bilingual, with British English and Brazilian Portuguese text.

``vmm::error::Result<T>``
   Value-or-error helper for APIs where exceptions are not desired.
