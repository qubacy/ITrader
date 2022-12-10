#include "DatabaseFacadeIntellectualEditor_test.h"
#include "qtestcase.h"

DatabaseFacadeIntellectualEditor_test::DatabaseFacadeIntellectualEditor_test(QObject *parent)
    : QObject{parent}
{
    
}

void DatabaseFacadeIntellectualEditor_test::initTestCase()
{
    std::shared_ptr<DatabaseSettings> dbSettings{std::make_shared<DatabaseSettings>()};
    
    dbSettings->load();
    
    auto driver = DatabaseDriverFabric::createDriverWithSettings(dbSettings);
    
    QVERIFY(driver.get() != nullptr);
    
    std::shared_ptr<DatabaseQueryProcessor> queryProcessor{nullptr};
    
    QVERIFY(DatabaseQueryProcessorFabric::createQueryProcessorWithSettings(dbSettings, std::move(driver), queryProcessor));
    
    std::unique_ptr<DatabaseFacadeIntellectualEditor> dbFacade{std::make_unique<DatabaseFacadeIntellectualEditor>(queryProcessor)};
    
    QVERIFY(dbFacade.get() != nullptr);
    
    m_dbFacade = std::move(dbFacade);
    
    std::vector<std::shared_ptr<LinePattern>> linePatterns{};
    
    QVERIFY(m_dbFacade->getLinePatterns(linePatterns).get() == nullptr);
    
    LinePatternStore::init();
    
    auto linePatternStore = LinePatternStore::getInstance();
    
    QVERIFY(linePatternStore.get() != nullptr);
    QVERIFY(linePatternStore->initLinePatterns(linePatterns));
    
    std::vector<std::shared_ptr<Pattern>> patterns{};
    
    QVERIFY(m_dbFacade->getPatterns(patterns).get() == nullptr);
    
    m_patterns = std::move(patterns);
}

void DatabaseFacadeIntellectualEditor_test::checkPatternInsert_test()
{
    std::vector<PatternLine> newPatternLines{PatternLine{0, 0, Line::Side::S_DOWN}};
    std::shared_ptr<Pattern> newPattern{std::make_shared<Pattern>("newPattern", 
                                                                  0.6, 
                                                                  Pattern::Trend::T_UP, 
                                                                  Pattern::Trend::T_UP, 
                                                                  std::move(newPatternLines))};
    
    QVERIFY(m_dbFacade->insertPattern(newPattern).get() == nullptr);
}

/*
 * PatternLines should be deleted in CASCADE way by Database itself!
*/

void DatabaseFacadeIntellectualEditor_test::checkPatternDelete_test()
{
    auto patternDoDelete{m_patterns.back()};
    
    QVERIFY(m_dbFacade->deletePattern(patternDoDelete).get() == nullptr);
}
