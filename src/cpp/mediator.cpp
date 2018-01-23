#include "mediator.h"

Mediator::Mediator(QObject *parent) : QObject(parent)
{
    // nothing to do
}

EView::View Mediator::activeView()
{
    return m_activeView;
}

void Mediator::requestNewView(int view)
{
    setActiveView(static_cast<EView::View>(view));
}

void Mediator::setActiveView(EView::View activeView)
{
    if (m_activeView != activeView) {
        m_activeView = activeView;
        emit activeViewChanged(activeView);
    }
}
