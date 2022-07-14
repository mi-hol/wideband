#include "can_helper.h"

#include <cstring>

// manualy rotate mailboxes on GD32
static int can1_mailbox = 1;

CanTxMessage::CanTxMessage(uint32_t eid, uint8_t dlc, bool isExtended) {
    m_frame.IDE = isExtended ? CAN_IDE_EXT : CAN_IDE_STD;
    m_frame.EID = eid;
    m_frame.RTR = CAN_RTR_DATA;
    m_frame.DLC = dlc;
    memset(m_frame.data8, 0, sizeof(m_frame.data8));
}

CanTxMessage::~CanTxMessage() {
    // 100 ms timeout
    canTransmitTimeout(&CAND1, can1_mailbox, &m_frame, TIME_MS2I(100));

    // GD32 workaround: rotate mailboxes
    // Does not affect STM32 devices
    can1_mailbox++;
    if (can1_mailbox == CAN_TX_MAILBOXES) {
        can1_mailbox = 1;
    }
}

uint8_t& CanTxMessage::operator[](size_t index) {
    return m_frame.data8[index];
}
