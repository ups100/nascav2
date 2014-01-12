/**
 * @file  LogsTransmitter.cpp
 * @brief  Definition of the Class INZ_project::TcpStandardModule::SessionPart
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "LogsTransmitter.h"
#include "MessageFormer.h"
#include "MessageCodes.h"
#include "LogEntry.h"
#include "Session.h"
#include "DataPortion.h"
#include "ClientSession.h"
#include "DataChannel.h"
#include "Log.h"
#include <QtEndian>

using INZ_project::Base::DataPortion;
using INZ_project::Base::Log;

namespace INZ_project {
namespace TcpStandardModule {

LogsTransmitter::LogsTransmitter(Base::DataProvider *provider, Session *session,
        int milis)
        : SessionPart(session, milis), m_isStarted(false), m_channel(0L),
                m_provider(provider), m_state(NONE)
{
    m_timer.setInterval(milis);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(requestTimedOut()));

}

LogsTransmitter::~LogsTransmitter()
{

}

SessionPart *LogsTransmitter::clone()
{
    return m_isStarted ?
            0L : new LogsTransmitter(m_provider, m_session, m_timer.interval());
}

void LogsTransmitter::execute()
{
    if (m_isStarted) {
        return;
    }

    m_isStarted = true;
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();

    connect(destination.get(), SIGNAL(readyRead()), this,
            SLOT(readNextMessage()));
    connect(destination.get(), SIGNAL(error(MessageSink::SinkError)), this,
            SLOT(sinkError(MessageSink::SinkError)));
    connect(destination.get(), SIGNAL(sinkClosed()), this, SLOT(sinkClosed()));

    if ((m_channel = m_session->getClient()->getDataChannel()) != 0L) {
        //we have data channel so let's notify client that we are waiting for logs
        destination->write(
                MessageCodes::getMessageCode(MessageCodes::WAITING_FOR_LOGS));
        m_state = WAITING_FOR_LOG;
        m_timer.start();
    } else {
        LOG_ENTRY(MyLogger::INFO,
                "Client: "<<m_session->getClient()->getClientId()<<"should not use this provider");

        destination->write(MessageCodes::getMessageCode(MessageCodes::NACK));

        //Finish successfully because it's not an error all part were correct, there is only no route
        disconnectAll();
        emit finished(true);
    }
}

void LogsTransmitter::terminate()
{
    disconnectAll();

    emit finished(false);
}

void LogsTransmitter::sinkError(MessageSink::SinkError error)
{
    LOG_ENTRY(MyLogger::DEBUG, "Error #"<<error<<"in MessageSink.");
    disconnectAll();

    emit finished(false);
}

void LogsTransmitter::readNextMessage()
{
    m_timer.stop();
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();
    QByteArray message = destination->read();

    switch (MessageCodes::getMessageType(message)) {
        case MessageCodes::LOGS_PORTION:
            logsPortionReceived(message);
            break;

        case MessageCodes::END:
            disconnectAll();
            emit finished(true);
            break;

        default:
            LOG_ENTRY(MyLogger::DEBUG,
                    "Unexpected message: #"<<MessageCodes::getMessageType(message) <<" received from client.");
            //close the session
            disconnectAll();
            emit finished(false);
            break;
    }
}

void LogsTransmitter::logsPortionReceived(const QByteArray& message)
{
    QList<boost::shared_ptr<Log> > logs;
    //host Structure: <byte> <8 bytes> <null term> byte    <null term>
    //host             HOST_CHECK   time_t    hostName    state     output
    //service structure: <byte> <8 bytes> <null term> <null term> byte  <null term>
    //service structure: SRV_CHECK    timet_t    hostName    srvName  state  output

    int prevIndex = 1;
    //parse the logs
    for (QByteArray tmpMessage = message.mid(1); tmpMessage.size() > 0;) {
        MessageCodes::LogType type = MessageCodes::getLogType(tmpMessage);
        if (type == MessageCodes::INVALID_LOG_TYPE) {
            //error
            LOG_ENTRY(MyLogger::ERROR, "Wrong Logs format");
            disconnectAll();
            emit finished(false);
            return;
        }

        tmpMessage = tmpMessage.mid(1);

        //Let's get the time
        qint64 rawTime = qFromBigEndian<qint64>(
                (const uchar*) tmpMessage.data());
        time_t time = translateTime(rawTime);
        tmpMessage = tmpMessage.mid(8);

        //now lets get the the host name
        QString hostName = getNullTerminatedString(tmpMessage);
        tmpMessage = tmpMessage.mid(hostName.size() + 1);

        //get the service name if suitable
        QString serviceName;
        if (type == MessageCodes::SERVICE_CHECK) {
            serviceName = getNullTerminatedString(tmpMessage);
            tmpMessage = tmpMessage.mid(serviceName.size() + 1);
        }

        //get the state
        char state = tmpMessage[0];
        tmpMessage = tmpMessage.mid(1);

        //get the plugin output
        QString pluginOutput = getNullTerminatedString(tmpMessage);
        tmpMessage = tmpMessage.mid(pluginOutput.size() + 1);

        try {
            Log *log = new Log(time, hostName, serviceName, state,
                    pluginOutput);
            logs.append(boost::shared_ptr<Log>(log));
        } catch (Log::LogWrongFormatException &e) {
            LOG_ENTRY(MyLogger::ERROR, "Log wrong format: "<<e.what());
            disconnectAll();
            emit finished(false);
            return;
        }

    }

    if (logs.empty()) {
        LOG_ENTRY(MyLogger::ERROR, "Empty logs package received");
        disconnectAll();
        emit finished(false);
        return;
    }

    try {
        //initialize portion
        DataPortion portion(logs, m_session->getClient().get(), m_provider);

        //count the hash
        QByteArray hash = m_session->getHashAlgorithm()->generateHash(
                message.mid(1));

        qint64 id = 0;
        if ((id = m_channel->scheduleForWrite(portion)) == 0) {
            //logs hash been submitted let's send confirmation
            boost::shared_ptr<MessageSink> destination = m_session
                    ->getDestination();
            destination->write(
                    MessageCodes::getMessageCode(MessageCodes::ACK) + hash);
            m_timer.start();

        } else if (id > 0) {
            m_state = WRITTING_LOG;
            m_request = std::make_pair(hash, id);

            connect(m_channel, SIGNAL(portionWritten(qint64)), this,
                    SLOT(portionWritten(qint64)));

        } else {
            LOG_ENTRY(MyLogger::ERROR, "Unable to write logs.");
            disconnectAll();
            emit finished(false);
        }
    } catch (DataPortion::NoRightsException &e) {
        LOG_ENTRY(MyLogger::ERROR, e.what());
        disconnectAll();
        emit finished(false);
        return;
    }

}

QString LogsTransmitter::getNullTerminatedString(const QByteArray& array)
{
    int pos = array.indexOf('\0');
    return pos < 0 ? QString() : QString::fromUtf8(array.left(pos));
}

time_t LogsTransmitter::translateTime(qint64 time)
{
    return (time_t) time;
}

void LogsTransmitter::portionWritten(qint64 id)
{
    if (m_state != WRITTING_LOG) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received logs confirmation in state #"<< m_state);
        disconnectAll();
        emit finished(false);
    }

    if (id == m_request.second) {
        m_state = WAITING_FOR_LOG;
        disconnect(m_channel, SIGNAL(portionWritten(qint64)), this,
                SLOT(portionWritten(qint64)));

        boost::shared_ptr<MessageSink> destination =
                m_session->getDestination();
        destination->write(
                MessageCodes::getMessageCode(MessageCodes::ACK)
                        + m_request.first);
        m_timer.start();
    }
}

void LogsTransmitter::sinkClosed()
{
    LOG_ENTRY(MyLogger::DEBUG, "Sink closed unexpectedly.");
    disconnectAll();

    emit finished(false);
}

void LogsTransmitter::requestTimedOut()
{
    LOG_ENTRY(MyLogger::DEBUG, "No response from client. Timeout.");
    disconnectAll();

    emit finished(false);
}

void LogsTransmitter::disconnectAll()
{
    m_timer.stop();
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();

    disconnect(destination.get(), SIGNAL(readyRead()), this,
            SLOT(readNextMessage()));
    disconnect(destination.get(), SIGNAL(error(MessageSink::SinkError)), this,
            SLOT(sinkError(MessageSink::SinkError)));
    disconnect(destination.get(), SIGNAL(sinkClosed()), this,
            SLOT(sinkClosed()));
    disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(requestTimedOut()));
    if (m_channel) {
        disconnect(m_channel, SIGNAL(portionWritten(qint64)), this,
                SLOT(portionWritten(qint64)));
    }

}

void LogsTransmitter::setTimeout(int milis)
{
    if (!m_isStarted) {
        m_timer.setInterval(milis);
    }
}

} //namespace TcpStandardModule
} //namespace INZ_project
