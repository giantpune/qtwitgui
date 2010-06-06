#include "filefolderdialog.h"

void FileFolderDialog::accept()
{
    QStringList files = selectedFiles();
    if (files.isEmpty())
	return;
    emit filesSelected(files);
    QDialog::accept();
}
