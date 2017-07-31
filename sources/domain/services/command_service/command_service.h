#ifndef COMMAND_SERVICE_H
#define COMMAND_SERVICE_H

// Qt
#include <QObject>

// Internal
#include "dao_traits.h"

namespace domain
{
    class Command;

    class CommandService: public QObject
    {
        Q_OBJECT

    public:
        explicit CommandService(QObject* parent = nullptr);
        ~CommandService() override;

        Command headCommand();
        bool hasCommand() const;

    public slots:
        void executeCommand(const Command& command);

    signals:
        void gotCommand();

    private:
        class Impl;
        QScopedPointer<Impl> const d;
    };
}

#endif // COMMAND_SERVICE_H